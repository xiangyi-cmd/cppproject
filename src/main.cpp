#include "HotelManager.h"
#include <iostream>
#include <limits>
#include <string>

// 防呆輸入整數的輔助函式
int getValidIntInput(const std::string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            if (value >= minVal && value <= maxVal) {
                // 清理緩衝區中殘留的換行符號
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            } else {
                std::cout << "\033[31m錯誤：請輸入介於 " << minVal << " 到 " << maxVal << " 之間的數字！\033[0m\n";
            }
        } else {
            std::cout << "\033[31m錯誤：請輸入正確的數字格式！\033[0m\n";
            std::cin.clear(); // 重設 cin 錯誤狀態
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清除錯誤字串
        }
    }
}

// 防呆輸入浮點數的輔助函式
double getValidDoubleInput(const std::string& prompt, double minVal) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            if (value >= minVal) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            } else {
                std::cout << "\033[31m錯誤：輸入值不得小於 " << minVal << "！\033[0m\n";
            }
        } else {
            std::cout << "\033[31m錯誤：請輸入正確的數值格式！\033[0m\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

// 取得非空字串輸入的輔助函式
std::string getNonEmptyStringInput(const std::string& prompt) {
    std::string value;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, value);
        
        // 移除字串前後的空白 (Trimming)
        if (!value.empty() && value.find_first_not_of(" \t\r\n") != std::string::npos) {
            return value;
        }
        std::cout << "\033[31m錯誤：輸入欄位不可為空，請重新輸入！\033[0m\n";
    }
}

int main() {
    HotelManager manager;
    const std::string roomsFile = "data/rooms.csv";
    const std::string bookingsFile = "data/bookings.csv";

    std::cout << "\033[34m============================================\033[0m\n";
    std::cout << "\033[36m    歡迎使用「智慧型訂房與客房管理系統」    \033[0m\n";
    std::cout << "\033[34m============================================\033[0m\n";

    // 系統載入
    std::cout << "正在載入客房與預訂資料...\n";
    if (!manager.loadRooms(roomsFile)) {
        std::cout << "\033[33m警告：找不到 " << roomsFile << "，將初始化預設房間資料。\033[0m\n";
    } else {
        std::cout << "客房資料載入成功！\n";
    }

    if (!manager.loadBookings(bookingsFile)) {
        std::cout << "\033[33m提示：無歷史訂單檔案，將於首次存檔時建立。\033[0m\n";
    } else {
        std::cout << "歷史預訂紀錄載入成功！\n";
    }

    while (true) {
        std::cout << "\n\033[34m================== 主選單 ==================\033[0m\n";
        std::cout << "  1. 查看所有客房狀態 (List All Rooms)\n";
        std::cout << "  2. 條件篩選與搜尋 (Search & Filter)\n";
        std::cout << "  3. 辦理入住 / 預訂房間 (Book a Room)\n";
        std::cout << "  4. 辦理退房結帳 (Check-Out & Invoice)\n";
        std::cout << "  5. 查看飯店營運統計 (View Statistics)\n";
        std::cout << "  6. 儲存並退出系統 (Save & Exit)\n";
        std::cout << "\033[34m============================================\033[0m\n";

        int choice = getValidIntInput("請輸入您的選擇 (1-6): ", 1, 6);

        switch (choice) {
            case 1: {
                std::cout << "\n\033[36m[查看房間狀態]\033[0m\n";
                std::cout << "1. 依房號排序 (預設)\n";
                std::cout << "2. 依房價由低到高排序\n";
                int sortChoice = getValidIntInput("請選擇排序方式: ", 1, 2);
                manager.displayAllRooms(sortChoice == 2);
                break;
            }
            case 2: {
                std::cout << "\n\033[36m[條件篩選與搜尋]\033[0m\n";
                std::cout << "1. 篩選單人房 (Single)\n";
                std::cout << "2. 篩選雙人房 (Double)\n";
                std::cout << "3. 篩選總統套房 (Suite)\n";
                std::cout << "4. 不限房型\n";
                int typeChoice = getValidIntInput("請選擇房型: ", 1, 4);
                
                std::string typeFilter = "";
                if (typeChoice == 1) typeFilter = "Single";
                else if (typeChoice == 2) typeFilter = "Double";
                else if (typeChoice == 3) typeFilter = "Suite";

                double maxPrice = getValidDoubleInput("請輸入預算上限 (輸入 0 表示不設上限): ", 0);
                
                std::cout << "是否僅顯示空閒房間？\n";
                std::cout << "1. 是 (僅空房)\n";
                std::cout << "2. 否 (顯示全部)\n";
                int availChoice = getValidIntInput("請選擇: ", 1, 2);

                manager.searchRooms(typeFilter, maxPrice, availChoice == 1);
                break;
            }
            case 3: {
                std::cout << "\n\033[36m[辦理入住 / 預訂房間]\033[0m\n";
                int roomNum = getValidIntInput("請輸入欲預訂的房號: ", 100, 999);
                
                // 先檢查房間是否存在且空閒
                auto roomObj = manager.findRoom(roomNum);
                if (!roomObj) {
                    std::cout << "\033[31m錯誤：不存在此房號！\033[0m\n";
                    break;
                }
                if (roomObj->getIsBooked()) {
                    std::cout << "\033[31m錯誤：此房間已經被預訂！請查看房態牆選擇其他房間。\033[0m\n";
                    break;
                }

                // 顯示該房型明細
                std::cout << "\n\033[33m--- 客房詳細資訊 ---\033[0m\n";
                roomObj->displayRoomDetails();
                std::cout << "---------------------\n";

                std::string guestName = getNonEmptyStringInput("請輸入住客姓名: ");
                int nights = getValidIntInput("請輸入預計入住天數 (1-30天): ", 1, 30);

                manager.bookRoom(roomNum, guestName, nights);
                break;
            }
            case 4: {
                std::cout << "\n\033[36m[辦理退房結帳]\033[0m\n";
                int roomNum = getValidIntInput("請輸入退房的房號: ", 100, 999);
                
                double cost = 0.0;
                if (manager.checkoutRoom(roomNum, cost)) {
                    std::cout << "\033[32m退房手續完成！房間已釋放為空房。\033[0m\n";
                }
                break;
            }
            case 5: {
                manager.showStatistics();
                break;
            }
            case 6: {
                std::cout << "\n正在儲存資料至 CSV 檔案...\n";
                if (manager.saveRooms(roomsFile) && manager.saveBookings(bookingsFile)) {
                    std::cout << "\033[32m資料儲存成功！\033[0m\n";
                } else {
                    std::cout << "\033[31m錯誤：儲存資料時發生問題！\033[0m\n";
                }
                std::cout << "感謝您的使用，再見！\n";
                return 0;
            }
        }
        
        // 每次功能結束後，要求使用者按 Enter 繼續，讓畫面可控
        std::cout << "\n按 Enter 鍵回主選單...";
        std::cin.get();
    }

    return 0;
}
