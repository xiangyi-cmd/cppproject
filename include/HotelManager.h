#ifndef HOTEL_MANAGER_H
#define HOTEL_MANAGER_H

#include "Room.h"
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

// 歷史與有效訂單紀錄結構 (STL Struct)
struct BookingRecord {
    std::string bookingId;
    std::string guestName;
    int roomNumber;
    std::string roomType;
    int nights;
    double totalCost;
};

// 飯店管理核心類別 (Hotel Manager Class)
class HotelManager {
private:
    std::vector<std::shared_ptr<Room>> rooms; // 使用 std::vector 儲存多型物件指標
    std::vector<BookingRecord> bookings;      // 使用 std::vector 儲存歷史訂單紀錄

    // 私有輔助函式：產生隨機且唯一的訂單編號
    std::string generateBookingId() const;

public:
    HotelManager() = default;
    ~HotelManager() = default;

    // 1. 檔案讀寫核心 (File I/O)
    bool loadRooms(const std::string& filepath);
    bool saveRooms(const std::string& filepath) const;
    bool loadBookings(const std::string& filepath);
    bool saveBookings(const std::string& filepath) const;

    // 2. 房間操作功能 (Room Operations)
    void displayAllRooms(bool sortByPrice = false) const;
    void searchRooms(const std::string& typeFilter, double maxPrice, bool onlyAvailable) const;
    bool bookRoom(int roomNum, const std::string& name, int nights);
    bool checkoutRoom(int roomNum, double& finalCost);

    // 3. 營運統計功能 (Statistics)
    void showStatistics() const;

    // 4. 輔助尋找房間指標
    std::shared_ptr<Room> findRoom(int roomNum) const;
    const std::vector<std::shared_ptr<Room>>& getAllRooms() const;
};

#endif // HOTEL_MANAGER_H
