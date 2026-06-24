#include "Room.h"

// ==================== Room (基底類別) 實作 ====================

Room::Room(int num, double price) 
    : roomNumber(num), basePrice(price), isBooked(false), guestName(""), bookingNights(0) {}

int Room::getRoomNumber() const { return roomNumber; }
double Room::getBasePrice() const { return basePrice; }
bool Room::getIsBooked() const { return isBooked; }
std::string Room::getGuestName() const { return guestName; }
int Room::getBookingNights() const { return bookingNights; }

void Room::bookRoom(const std::string& gName, int nights) {
    isBooked = true;
    guestName = gName;
    bookingNights = nights;
}

void Room::checkoutRoom() {
    isBooked = false;
    guestName = "";
    bookingNights = 0;
}


// ==================== SingleRoom (單人房) 實作 ====================

SingleRoom::SingleRoom(int num) : Room(num, 1000.0) {}

double SingleRoom::calculateTotalCost(int nights) const {
    return basePrice * nights;
}

void SingleRoom::displayRoomDetails() const {
    std::cout << "房號: " << roomNumber << "\n"
              << "類型: 單人房 (Single Room)\n"
              << "基本房價: $" << basePrice << " / 晚\n"
              << "預訂狀態: " << (isBooked ? "已預訂 (住客: " + guestName + ", 天數: " + std::to_string(bookingNights) + " 晚)" : "空閒 (Available)") << "\n"
              << "備註: 簡約大方，適合單人出差旅遊。\n";
}

std::string SingleRoom::getRoomType() const {
    return "Single";
}


// ==================== DoubleRoom (雙人房) 實作 ====================

DoubleRoom::DoubleRoom(int num) : Room(num, 1800.0) {}

double DoubleRoom::calculateTotalCost(int nights) const {
    // 總價 = (基本房價 + 雙人早餐費 + 服務費) * 天數
    return (basePrice + breakfastFeePerNight + serviceFeePerNight) * nights;
}

void DoubleRoom::displayRoomDetails() const {
    std::cout << "房號: " << roomNumber << "\n"
              << "類型: 雙人房 (Double Room)\n"
              << "基本房價: $" << basePrice << " / 晚\n"
              << "附加費用 (每日): 雙人份活力早餐 ($" << breakfastFeePerNight << ")、服務費 ($" << serviceFeePerNight << ")\n"
              << "預訂狀態: " << (isBooked ? "已預訂 (住客: " + guestName + ", 天數: " + std::to_string(bookingNights) + " 晚)" : "空閒 (Available)") << "\n"
              << "備註: 寬敞雙人床，包含活力雙人早餐。\n";
}

std::string DoubleRoom::getRoomType() const {
    return "Double";
}


// ==================== Suite (總統套房) 實作 ====================

Suite::Suite(int num) : Room(num, 5000.0) {}

double Suite::calculateTotalCost(int nights) const {
    // 總價 = ((基本房價 * 天數) + 貴賓室使用費) * 奢侈稅 1.1
    return ((basePrice * nights) + vipLoungeAccessFee) * luxuryTaxRate;
}

void Suite::displayRoomDetails() const {
    std::cout << "房號: " << roomNumber << "\n"
              << "類型: 總統套房 (Suite Room)\n"
              << "基本房價: $" << basePrice << " / 晚\n"
              << "附加費用: 一次性 VIP 貴賓室使用費 ($" << vipLoungeAccessFee << ")、奢侈稅 (加收總額 10%)\n"
              << "預訂狀態: " << (isBooked ? "已預訂 (住客: " + guestName + ", 天數: " + std::to_string(bookingNights) + " 晚)" : "空閒 (Available)") << "\n"
              << "備註: 豪華行政套房，享專屬高空貴賓室使用權。\n";
}

std::string Suite::getRoomType() const {
    return "Suite";
}
