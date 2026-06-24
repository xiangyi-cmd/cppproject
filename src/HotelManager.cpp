#include "HotelManager.h"
#include <ctime>
#include <iomanip>

// 產生唯一的訂單編號
std::string HotelManager::generateBookingId() const {
    // 使用當前時間戳作為訂單編號的一部分，保證不重複
    return "BK_" + std::to_string(std::time(nullptr));
}

// 尋找特定房號的房間指標 (使用 STL Algorithm `find_if`)
std::shared_ptr<Room> HotelManager::findRoom(int roomNum) const {
    auto it = std::find_if(rooms.begin(), rooms.end(), [roomNum](const std::shared_ptr<Room>& r) {
        return r->getRoomNumber() == roomNum;
    });
    
    if (it != rooms.end()) {
        return *it;
    }
    return nullptr;
}

// 1. 載入客房狀態檔 (File I/O)
bool HotelManager::loadRooms(const std::string& filepath) {
    rooms.clear();
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string roomNumStr, type, priceStr, bookedStr, gName, nightsStr;

        std::getline(ss, roomNumStr, ',');
        std::getline(ss, type, ',');
        std::getline(ss, priceStr, ',');
        std::getline(ss, bookedStr, ',');
        std::getline(ss, gName, ',');
        std::getline(ss, nightsStr, ',');

        if (roomNumStr.empty() || type.empty()) continue;

        int num = std::stoi(roomNumStr);
        int booked = std::stoi(bookedStr);
        int nights = nightsStr.empty() ? 0 : std::stoi(nightsStr);

        std::shared_ptr<Room> r = nullptr;
        if (type == "Single") {
            r = std::make_shared<SingleRoom>(num);
        } else if (type == "Double") {
            r = std::make_shared<DoubleRoom>(num);
        } else if (type == "Suite") {
            r = std::make_shared<Suite>(num);
        }

        if (r) {
            if (booked) {
                r->bookRoom(gName, nights);
            }
            rooms.push_back(r);
        }
    }
    file.close();
    return true;
}

// 儲存客房狀態檔 (File I/O)
bool HotelManager::saveRooms(const std::string& filepath) const {
    std::ofstream file(filepath);
    if (!file.is_open()) return false;

    for (const auto& r : rooms) {
        file << r->getRoomNumber() << ","
             << r->getRoomType() << ","
             << r->getBasePrice() << ","
             << (r->getIsBooked() ? 1 : 0) << ","
             << r->getGuestName() << ","
             << r->getBookingNights() << "\n";
    }
    file.close();
    return true;
}

// 載入歷史訂單檔 (File I/O)
bool HotelManager::loadBookings(const std::string& filepath) {
    bookings.clear();
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    std::string line;
    // 讀取並略過第一行 CSV 表頭
    if (std::getline(file, line)) {
        if (line.find("BookingID") == std::string::npos && !line.empty()) {
            // 如果這行沒有欄位標題，表示是資料，需重新解析
            std::stringstream ss(line);
            std::string bid, gname, rnumStr, rtype, nightsStr, costStr;
            std::getline(ss, bid, ',');
            std::getline(ss, gname, ',');
            std::getline(ss, rnumStr, ',');
            std::getline(ss, rtype, ',');
            std::getline(ss, nightsStr, ',');
            std::getline(ss, costStr, ',');
            if (!bid.empty()) {
                BookingRecord record{bid, gname, std::stoi(rnumStr), rtype, std::stoi(nightsStr), std::stod(costStr)};
                bookings.push_back(record);
            }
        }
    }

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string bid, gname, rnumStr, rtype, nightsStr, costStr;

        std::getline(ss, bid, ',');
        std::getline(ss, gname, ',');
        std::getline(ss, rnumStr, ',');
        std::getline(ss, rtype, ',');
        std::getline(ss, nightsStr, ',');
        std::getline(ss, costStr, ',');

        if (bid.empty()) continue;

        BookingRecord record{
            bid,
            gname,
            std::stoi(rnumStr),
            rtype,
            std::stoi(nightsStr),
            std::stod(costStr)
        };
        bookings.push_back(record);
    }
    file.close();
    return true;
}

// 儲存歷史訂單檔 (File I/O)
bool HotelManager::saveBookings(const std::string& filepath) const {
    std::ofstream file(filepath);
    if (!file.is_open()) return false;

    // 寫入 CSV 標頭
    file << "BookingID,GuestName,RoomNumber,RoomType,Nights,TotalCost\n";

    for (const auto& b : bookings) {
        file << b.bookingId << ","
             << b.guestName << ","
             << b.roomNumber << ","
             << b.roomType << ","
             << b.nights << ","
             << b.totalCost << "\n";
    }
    file.close();
    return true;
}

// 2. 顯示所有房間狀態 (使用 STL 演算法進行排序)
void HotelManager::displayAllRooms(bool sortByPrice) const {
    auto roomsCopy = rooms; // 複製一份以進行排序，避免更動原始順序
    
    if (sortByPrice) {
        // 使用 STL sort 依據「價格」由低到高排序
        std::sort(roomsCopy.begin(), roomsCopy.end(), [](const std::shared_ptr<Room>& a, const std::shared_ptr<Room>& b) {
            return a->getBasePrice() < b->getBasePrice();
        });
    } else {
        // 預設依據「房號」排序
        std::sort(roomsCopy.begin(), roomsCopy.end(), [](const std::shared_ptr<Room>& a, const std::shared_ptr<Room>& b) {
            return a->getRoomNumber() < b->getRoomNumber();
        });
    }

    std::cout << "\n\033[36m========================================================================\033[0m\n";
    std::cout << std::left << std::setw(10) << "房號" 
              << std::setw(15) << "客房類型" 
              << std::setw(15) << "基本房價" 
              << std::setw(15) << "狀態" 
              << "住客姓名 (入住天數)\n";
    std::cout << "\033[36m------------------------------------------------------------------------\033[0m\n";

    for (const auto& r : roomsCopy) {
        std::string statusStr = r->getIsBooked() ? "\033[31m[已預訂]\033[0m" : "\033[32m[空閒]\033[0m";
        std::string typeChinese = "";
        
        if (r->getRoomType() == "Single") typeChinese = "單人房";
        else if (r->getRoomType() == "Double") typeChinese = "雙人房";
        else if (r->getRoomType() == "Suite") typeChinese = "總統套房";

        std::cout << std::left << std::setw(10) << r->getRoomNumber() 
                  << std::setw(17) << typeChinese // 調整中文字元寬度
                  << "$" << std::setw(13) << (int)r->getBasePrice()
                  << statusStr << "    ";
        
        if (r->getIsBooked()) {
            std::cout << r->getGuestName() << " (" << r->getBookingNights() << " 晚)";
        }
        std::cout << "\n";
    }
    std::cout << "\033[36m========================================================================\033[0m\n";
}

// 搜尋符合條件之房間 (使用 STL 篩選)
void HotelManager::searchRooms(const std::string& typeFilter, double maxPrice, bool onlyAvailable) const {
    std::cout << "\n\033[33m符合搜尋條件的房間列表：\033[0m\n";
    std::cout << std::left << std::setw(10) << "房號" 
              << std::setw(15) << "客房類型" 
              << std::setw(15) << "基本房價" 
              << "狀態\n";
    std::cout << "-----------------------------------------------------\n";

    int count = 0;
    for (const auto& r : rooms) {
        // 條件篩選
        if (!typeFilter.empty() && r->getRoomType() != typeFilter) continue;
        if (maxPrice > 0 && r->getBasePrice() > maxPrice) continue;
        if (onlyAvailable && r->getIsBooked()) continue;

        std::string statusStr = r->getIsBooked() ? "\033[31m[已預訂]\033[0m" : "\033[32m[空閒]\033[0m";
        std::string typeChinese = "";
        if (r->getRoomType() == "Single") typeChinese = "單人房";
        else if (r->getRoomType() == "Double") typeChinese = "雙人房";
        else if (r->getRoomType() == "Suite") typeChinese = "總統套房";

        std::cout << std::left << std::setw(10) << r->getRoomNumber() 
                  << std::setw(17) << typeChinese
                  << "$" << std::setw(13) << (int)r->getBasePrice()
                  << statusStr << "\n";
        count++;
    }
    
    if (count == 0) {
        std::cout << "  (無符合篩選條件的客房)\n";
    }
    std::cout << "-----------------------------------------------------\n";
}

// 辦理預訂
bool HotelManager::bookRoom(int roomNum, const std::string& name, int nights) {
    auto r = findRoom(roomNum);
    if (!r) {
        std::cout << "\033[31m錯誤：找不到房號 " << roomNum << " 的房間！\033[0m\n";
        return false;
    }
    if (r->getIsBooked()) {
        std::cout << "\033[31m錯誤：房號 " << roomNum << " 已經被預訂了！\033[0m\n";
        return false;
    }

    // 進行預訂
    r->bookRoom(name, nights);
    double cost = r->calculateTotalCost(nights);

    // 寫入訂單紀錄
    BookingRecord rec{
        generateBookingId(),
        name,
        roomNum,
        r->getRoomType(),
        nights,
        cost
    };
    bookings.push_back(rec);

    std::cout << "\n\033[32m🎉 預訂成功！已產生訂單資訊：\033[0m\n";
    std::cout << "----------------------------------------\n";
    std::cout << "訂單編號: " << rec.bookingId << "\n";
    std::cout << "住客姓名: " << name << " 先生/女士\n";
    std::cout << "房號: " << roomNum << " (" << (r->getRoomType() == "Single" ? "單人房" : (r->getRoomType() == "Double" ? "雙人房" : "總統套房")) << ")\n";
    std::cout << "入住天數: " << nights << " 晚\n";
    std::cout << "總費用：$" << std::fixed << std::setprecision(0) << cost << " 元\n";
    std::cout << "----------------------------------------\n";
    
    return true;
}

// 辦理退房
bool HotelManager::checkoutRoom(int roomNum, double& finalCost) {
    auto r = findRoom(roomNum);
    if (!r) {
        std::cout << "\033[31m錯誤：找不到房號 " << roomNum << " 的房間！\033[0m\n";
        return false;
    }
    if (!r->getIsBooked()) {
        std::cout << "\033[31m錯誤：房號 " << roomNum << " 目前為空房，不需退房！\033[0m\n";
        return false;
    }

    finalCost = r->calculateTotalCost(r->getBookingNights());
    std::cout << "\n\033[33m================ 退房結帳明細 ================\033[0m\n";
    r->displayRoomDetails();
    std::cout << "預訂天數: " << r->getBookingNights() << " 晚\n";
    std::cout << "結帳總額: \033[32m$" << std::fixed << std::setprecision(0) << finalCost << "\033[0m 元\n";
    std::cout << "\033[33m==============================================\033[0m\n";

    // 清空客房狀態
    r->checkoutRoom();
    return true;
}

// 3. 顯示營運統計
void HotelManager::showStatistics() const {
    int total = rooms.size();
    int bookedCount = 0;
    double totalRev = 0.0;

    for (const auto& r : rooms) {
        if (r->getIsBooked()) {
            bookedCount++;
        }
    }

    for (const auto& b : bookings) {
        totalRev += b.totalCost;
    }

    double occupancy = total > 0 ? (double)bookedCount / total * 100.0 : 0.0;

    std::cout << "\n\033[35m================ 飯店營運統計報告 ================\033[0m\n";
    std::cout << "總客房數: " << total << " 間\n";
    std::cout << "已入住房: " << bookedCount << " 間\n";
    std::cout << "目前住房率: " << std::fixed << std::setprecision(1) << occupancy << " %\n";
    std::cout << "歷史累積營業額: \033[32m$" << std::fixed << std::setprecision(0) << totalRev << "\033[0m 元\n";
    std::cout << "歷史訂單總筆數: " << bookings.size() << " 筆\n";
    std::cout << "\033[35m==================================================\033[0m\n";
}
