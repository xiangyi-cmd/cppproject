#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <iostream>
#include <iomanip>

// 客房基底類別 (Base Class)
class Room {
protected:
    int roomNumber;
    double basePrice;
    bool isBooked;
    std::string guestName;
    int bookingNights;

public:
    Room(int num, double price);
    virtual ~Room() = default; // 虛擬解構子，確保子類別能正確釋放記憶體

    // 純虛擬函式 (Pure Virtual Functions) - 展現多型特徵
    virtual double calculateTotalCost(int nights) const = 0;
    virtual void displayRoomDetails() const = 0;
    virtual std::string getRoomType() const = 0;

    // 通用屬性存取函式 (Getters & Setters)
    int getRoomNumber() const;
    double getBasePrice() const;
    bool getIsBooked() const;
    std::string getGuestName() const;
    int getBookingNights() const;

    // 業務功能
    void bookRoom(const std::string& gName, int nights);
    void checkoutRoom();
};

// 1. 單人房衍生類別 (Derived Class)
class SingleRoom : public Room {
public:
    SingleRoom(int num);
    double calculateTotalCost(int nights) const override;
    void displayRoomDetails() const override;
    std::string getRoomType() const override;
};

// 2. 雙人房衍生類別 (Derived Class)
class DoubleRoom : public Room {
private:
    double breakfastFeePerNight = 250.0; // 雙人早餐費
    double serviceFeePerNight = 150.0;   // 服務費

public:
    DoubleRoom(int num);
    double calculateTotalCost(int nights) const override;
    void displayRoomDetails() const override;
    std::string getRoomType() const override;
};

// 3. 總統套房衍生類別 (Derived Class)
class Suite : public Room {
private:
    double vipLoungeAccessFee = 1000.0;  // VIP 貴賓室費
    double luxuryTaxRate = 1.1;          // 奢侈稅率 (10% 加收)

public:
    Suite(int num);
    double calculateTotalCost(int nights) const override;
    void displayRoomDetails() const override;
    std::string getRoomType() const override;
};

#endif // ROOM_H
