#pragma once
#include "network/base/InetAddress.h"
#include <unordered_map>
#include <mutex>
#include <thread>
#include "base/NonCopyable.h"
#include "base/Singeton.h"
#include <vector>
#include "network/base/InetAddress.h"
#include <string>
#include <memory>

namespace tmms
{
    namespace network
    {
        using InetAddressPtr = std::shared_ptr<InetAddress>;
        class DnsService : public tmms::base::NonCopyable
        {
        public:
            DnsService() = default;
            ~DnsService();

            void AddHost(const std::string &host);
            InetAddressPtr GetHostAddress(const std::string &host, int index);
            std::vector<InetAddressPtr> GetHostAddress(const std::string &host);
            void UpdateHost(const std::string &host, std::vector<InetAddressPtr> &list);
            std::unordered_map<std::string, std::vector<InetAddressPtr>> GetHosts();
            void SetDnsServiceParam(int32_t interval, int32_t sleep, int32_t retry);
            void Start();
            void Stop();
            void OnWork();
            static void GetHostInfo(const std::string &host, std::vector<InetAddressPtr> &list);

        private:
            std::thread thread_;
            bool running_{false};
            std::mutex lock_;
            std::unordered_map<std::string, std::vector<InetAddressPtr>> hosts_info_;
            int32_t retry_{3};
            int32_t sleep_{200};
            int32_t interval_{180 * 1000};
        };

#define sDnsService tmms::base::Singeton<DnsService>::Instance()
    }
}