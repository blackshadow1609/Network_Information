#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <bitset>

class NetworkCalculator {
private:
    unsigned int ip;      
    unsigned int mask;    
    int prefix;           

public:
    
    NetworkCalculator(const std::string& ipStr, const std::string& maskStr) {
        ip = stringToUint(ipStr);

        if (maskStr.find('.') != std::string::npos) {
            mask = stringToUint(maskStr);
            prefix = maskToPrefix(mask);
        }
        else {
            prefix = std::stoi(maskStr);
            mask = prefixToMask(prefix);
        }
    }

    unsigned int stringToUint(const std::string& ipStr) {
        std::vector<int> octets;
        std::stringstream ss(ipStr);
        std::string octet;

        while (std::getline(ss, octet, '.')) {
            octets.push_back(std::stoi(octet));
        }

        return (octets[0] << 24) | (octets[1] << 16) | (octets[2] << 8) | octets[3];
    }

    std::string uintToString(unsigned int ip) {
        return std::to_string((ip >> 24) & 0xFF) + "." +
            std::to_string((ip >> 16) & 0xFF) + "." +
            std::to_string((ip >> 8) & 0xFF) + "." +
            std::to_string(ip & 0xFF);
    }

    unsigned int prefixToMask(int prefix) {
        if (prefix == 0) return 0;
        return ~((1u << (32 - prefix)) - 1);
    }

    int maskToPrefix(unsigned int mask) {
        int prefix = 0;
        unsigned int temp = mask;

        while (temp & 0x80000000) {
            prefix++;
            temp <<= 1;
        }

        return prefix;
    }

    unsigned int getNetworkAddress() {
        return ip & mask;
    }

    unsigned int getBroadcastAddress() {
        return ip | ~mask;
    }

    unsigned long getTotalIPs() {
        return (1UL << (32 - prefix));
    }

    unsigned long getHostCount() {
        unsigned long total = getTotalIPs();
        return total > 2 ? total - 2 : total;
    }

    unsigned int getFirstHost() {
        return getNetworkAddress() + 1;
    }

    unsigned int getLastHost() {
        return getBroadcastAddress() - 1;
    }

    void printNetworkInfo() {
        std::cout << "=== Информация о сети ===" << std::endl;
        std::cout << "IP-адрес: " << uintToString(ip) << std::endl;
        std::cout << "Маска сети: " << uintToString(mask) << " (/" << prefix << ")" << std::endl;
        std::cout << "Адрес сети: " << uintToString(getNetworkAddress()) << std::endl;
        std::cout << "Широковещательный адрес: " << uintToString(getBroadcastAddress()) << std::endl;
        std::cout << "Первый доступный IP: " << uintToString(getFirstHost()) << std::endl;
        std::cout << "Последний доступный IP: " << uintToString(getLastHost()) << std::endl;
        std::cout << "Всего IP-адресов: " << getTotalIPs() << std::endl;
        std::cout << "Доступно узлов: " << getHostCount() << std::endl;

        std::cout << "\n=== Бинарное представление ===" << std::endl;
        std::cout << "IP-адрес:    " << std::bitset<32>(ip) << std::endl;
        std::cout << "Маска:       " << std::bitset<32>(mask) << std::endl;
        std::cout << "Сеть:        " << std::bitset<32>(getNetworkAddress()) << std::endl;
    }
};

int main() {
    setlocale(LC_ALL, "");

    std::string ip, mask;

    std::cout << "Калькулятор сетей IPv4" << std::endl;
    std::cout << "======================" << std::endl;

    std::cout << "Введите IP-адрес (формат: 192.168.1.1): ";
    std::cin >> ip;

    std::cout << "Введите маску (формат: 255.255.255.0 или 24): ";
    std::cin >> mask;

    try {
        NetworkCalculator calculator(ip, mask);
        calculator.printNetworkInfo();
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: Неверный формат ввода!" << std::endl;
        return 1;
    }

    return 0;
}