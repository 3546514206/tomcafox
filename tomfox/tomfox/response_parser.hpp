#pragma once

#include "picohttpparser.h"
#include <array>
#include <string_view>

namespace tomfox {
    constexpr const static size_t MAX_RESPONSE_SIZE = 1024 * 1024 * 10;

    class response_parser {
    public:
        response_parser() { buf_.resize(10 * 1024); }

        int parse(int last_len) {
            if (!copy_headers_.empty())
                copy_headers_.clear();

            int minor_version;

            num_headers_ = sizeof(headers_) / sizeof(headers_[0]);
            const char *msg;
            size_t msg_len;
            header_len_ =
                    phr_parse_response(buf_.data(), cur_size_, &minor_version, &status_,
                                       &msg, &msg_len, headers_, &num_headers_, last_len);
            msg_ = {msg, msg_len};
            auto header_value =
                    get_header_value(headers_, num_headers_, "content-length");
            if (header_value.empty()) {
                body_len_ = 0;
            } else {
                body_len_ = atoi(header_value.data());
            }

            return header_len_;
        }

        int parse(const char *buf, size_t cur_size, int last_len) {
            if (!copy_headers_.empty())
                copy_headers_.clear();

            int minor_version;

            num_headers_ = sizeof(headers_) / sizeof(headers_[0]);
            const char *msg;
            size_t msg_len;
            header_len_ =
                    phr_parse_response(buf, cur_size, &minor_version, &status_, &msg,
                                       &msg_len, headers_, &num_headers_, last_len);
            msg_ = {msg, msg_len};
            auto header_value =
                    get_header_value(headers_, num_headers_, "content-length");
            if (header_value.empty()) {
                body_len_ = 0;
            } else {
                body_len_ = atoi(header_value.data());
            }

            return header_len_;
        }

        bool at_capacity() { return (header_len_ + body_len_) > buf_.size(); }

        void expand() {
            for (size_t i = 0; i < num_headers_; i++) {
                copy_headers_.emplace_back(
                        std::string(headers_[i].name, headers_[i].name_len),
                        std::string(headers_[i].value, headers_[i].value_len));
            }

            buf_.resize(total_len());
        }

        bool has_body() const { return body_len_ != 0; }

        std::string_view message() { return msg_; }

        std::string_view body() {
            return std::string_view(buf_.data() + header_len_, body_len_);
        }

        std::string_view head() { return std::string_view(buf_.data(), header_len_); }

        std::string_view curr_content() {
            return std::string_view(buf_.data() + header_len_, cur_size_ - header_len_);
        }

        int body_len() const { return body_len_; }

        char *buffer() { return &buf_[cur_size_]; }

        size_t left_size() { return buf_.size() - cur_size_; }

        std::pair<phr_header *, size_t> get_headers() {
            if (copy_headers_.empty())
                return {headers_, num_headers_};

            num_headers_ = copy_headers_.size();
            for (size_t i = 0; i < num_headers_; i++) {
                headers_[i].name = copy_headers_[i].first.data();
                headers_[i].name_len = copy_headers_[i].first.size();
                headers_[i].value = copy_headers_[i].second.data();
                headers_[i].value_len = copy_headers_[i].second.size();
            }
            return {headers_, num_headers_};
        }

        bool update_size(size_t size) {
            cur_size_ += size;
            if (cur_size_ > MAX_RESPONSE_SIZE)
                return true;

            return false;
        }

        size_t current_size() const { return cur_size_; }

        void reset() {
            cur_size_ = 0;
            header_len_ = 0;
            body_len_ = 0;
            status_ = 0;
        }

        size_t total_len() { return header_len_ + body_len_; }

        bool has_recieved_all() { return (total_len() == current_size()); }

        int status() const { return status_; }

        void set_status(status_type status) { status_ = (int) status; }

        std::string_view get_header_value(std::string_view key) {
            if (copy_headers_.empty()) {
                for (size_t i = 0; i < num_headers_; i++) {
                    if (iequal(headers_[i].name, headers_[i].name_len, key.data()))
                        return std::string_view(headers_[i].value, headers_[i].value_len);
                }

                return {};
            }

            auto it = std::find_if(
                    copy_headers_.begin(), copy_headers_.end(), [this, key](auto &pair) {
                        if (iequal(pair.first.data(), pair.first.size(), key.data())) {
                            return true;
                        }

                        return false;
                    });

            if (it != copy_headers_.end()) {
                return (*it).second;
            }

            return {};
        }

        bool is_chunked() {
            if (has_length()) {
                return false;
            }

            auto transfer_encoding = get_header_value("transfer-encoding");
            if (transfer_encoding == "chunked"sv) {
                return true;
            }

            return false;
        }

        bool has_length() {
            auto header_value = get_header_value("content-length");
            return !header_value.empty();
        }

    private:
        std::string_view get_header_value(phr_header *headers, size_t num_headers,
                                          std::string_view key) {
            for (size_t i = 0; i < num_headers; i++) {
                if (iequal(headers[i].name, headers[i].name_len, key.data()))
                    return std::string_view(headers[i].value, headers[i].value_len);
            }

            return {};
        }

        bool iequal(const char *s, size_t l, const char *t) {
            if (strlen(t) != l)
                return false;

            for (size_t i = 0; i < l; i++) {
                if (std::tolower(s[i]) != std::tolower(t[i]))
                    return false;
            }

            return true;
        }

        size_t cur_size_ = 0;
        int header_len_ = 0;
        int body_len_ = 0;
        std::string_view msg_;
        size_t msg_len_;
        int status_ = 0;

        size_t num_headers_ = 0;
        struct phr_header headers_[100];
        std::vector<char> buf_;
        std::vector<std::pair<std::string, std::string>> copy_headers_;
    };
} // namespace tomfox