#include "HotelManager.h"
#include <iostream>
#include <limits>
#include <string>
#include <cstdlib>
#include <vector>

#ifdef _WIN32
#include <conio.h>
#endif

// 宣告清屏函式
void clearScreen();

// 使用方向鍵做選單選擇的輔助函式
int getMenuChoiceWithArrows(const std::string& title, const std::vector<std::string>& options) {
    int selected = 0;
    int numOptions = options.size();

#ifdef _WIN32
    while (true) {
        clearScreen();
        std::cout << "\033[34m" << title << "\033[0m\n";
        for (int i = 0; i < numOptions; ++i) {
            if (i == selected) {
                std::cout << " \033[36m=> " << options[i] << "\033[0m\n";
            } else {
                std::cout << "    " << options[i] << "\n";
            }
        }
        std::cout << "\033[34m============================================\033[0m\n";
        std::cout << "提示：使用 ↑ / ↓ 鍵移動，按 Enter 鍵確定選擇\n";

        int ch = _getch();
        if (ch == 224 || ch == 0) {
            int subCh = _getch();
            if (subCh == 72) { // 向上鍵
                selected = (selected - 1 + numOptions) % numOptions;
            } else if (subCh == 80) { // 向下鍵
                selected = (selected + 1) % numOptions;
            }
        } else if (ch == 13 || ch == 10) {
            return selected + 1;
        } else if (ch >= '1' && ch <= '0' + numOptions) {
            return ch - '0';
        }
    }
#else
    clearScreen();
    std::cout << "\033[34m" << title << "\033[0m\n";
    for (int i = 0; i < numOptions; ++i) {
        std::cout << "  " << options[i] << "\n";
    }
    std::cout << "\033[34m============================================\033[0m\n";
    int val = 0;
    while (true) {
        std::cout << "請輸入您的選擇 (1-" << numOptions << "): ";
        if (std::cin >> val && val >= 1 && val <= numOptions) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return val;
        }
        std::cout << "\033[31m錯誤：請輸入正確的選項！\033[0m\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
#endif
}

// 跨平台清屏功能
void clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

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

    std::vector<std::string> mainMenuOptions = {
        "1. 查看所有客房狀態 (List All Rooms)",
        "2. 條件篩選與搜尋 (Search & Filter)",
        "3. 辦理入住 / 預訂房間 (Book a Room)",
        "4. 辦理退房結帳 (Check-Out & Invoice)",
        "5. 查看飯店營運統計 (View Statistics)",
        "6. 儲存並退出系統 (Save & Exit)"
    };

    while (true) {
        int choice = getMenuChoiceWithArrows("================== 主選單 ==================", mainMenuOptions);

        switch (choice) {
            case 1: {
                std::vector<std::string> sortOptions = {
                    "1. 依房號排序 (預設)",
                    "2. 依房價由低到高排序"
                };
                int sortChoice = getMenuChoiceWithArrows("================== [查看房間狀態 - 排序方式] ==================", sortOptions);
                clearScreen();
                std::cout << "\n\033[36m[查看房間狀態]\033[0m\n";
                manager.displayAllRooms(sortChoice == 2);
                break;
            }
            case 2: {
                std::vector<std::string> typeOptions = {
                    "1. 篩選單人房 (Single)",
                    "2. 篩選雙人房 (Double)",
                    "3. 篩選總統套房 (Suite)",
                    "4. 不限房型"
                };
                int typeChoice = getMenuChoiceWithArrows("================== [條件篩選與搜尋 - 選擇房型] ==================", typeOptions);
                
                std::string typeFilter = "";
                if (typeChoice == 1) typeFilter = "Single";
                else if (typeChoice == 2) typeFilter = "Double";
                else if (typeChoice == 3) typeFilter = "Suite";

                clearScreen();
                std::cout << "\n\033[36m================== [條件篩選與搜尋 - 輸入預算] ==================\033[0m\n";
                double maxPrice = getValidDoubleInput("請輸入預算上限 (輸入 0 表示不設上限): ", 0);
                
                std::vector<std::string> availOptions = {
                    "1. 是 (僅空房)",
                    "2. 否 (顯示全部)"
                };
                int availChoice = getMenuChoiceWithArrows("================== [條件篩選與搜尋 - 顯示狀態] ==================", availOptions);

                clearScreen();
                std::cout << "\n\033[36m[條件篩選與搜尋結果]\033[0m\n";
                manager.searchRooms(typeFilter, maxPrice, availChoice == 1);
                break;
            }
            case 3: {
                clearScreen();
                std::cout << "\n\033[36m================== [辦理入住 / 預訂房間] ==================\033[0m\n";
                
                std::vector<std::shared_ptr<Room>> vacantRooms;
                for (const auto& r : manager.getAllRooms()) {
                    if (!r->getIsBooked()) {
                        vacantRooms.push_back(r);
                    }
                }

                if (vacantRooms.empty()) {
                    std::cout << "\n\033[33m提示：目前所有客房皆已滿房！無法辦理入住。\033[0m\n";
                    break;
                }

                std::vector<std::string> checkinOptions;
                for (const auto& r : vacantRooms) {
                    std::string typeChinese = "";
                    if (r->getRoomType() == "Single") typeChinese = "單人房";
                    else if (r->getRoomType() == "Double") typeChinese = "雙人房";
                    else if (r->getRoomType() == "Suite") typeChinese = "總統套房";

                    std::string opt = "房號 " + std::to_string(r->getRoomNumber()) + 
                                      " (" + typeChinese + ") - $" + std::to_string((int)r->getBasePrice()) + " / 晚";
                    checkinOptions.push_back(opt);
                }
                checkinOptions.push_back("返回主選單");

                int checkinChoice = getMenuChoiceWithArrows("================== [辦理入住 - 選擇預訂客房] ==================", checkinOptions);
                
                if (checkinChoice == (int)checkinOptions.size()) {
                    break;
                }

                auto roomObj = vacantRooms[checkinChoice - 1];
                int roomNum = roomObj->getRoomNumber();
                
                clearScreen();
                std::cout << "\n\033[36m================== [辦理入住 / 預訂房間] ==================\033[0m\n";
                std::cout << "\n\033[33m--- 客房詳細資訊 ---\033[0m\n";
                roomObj->displayRoomDetails();
                std::cout << "---------------------\n";

                std::string guestName = getNonEmptyStringInput("請輸入住客姓名: ");
                int nights = getValidIntInput("請輸入預計入住天數 (1-30天): ", 1, 30);

                manager.bookRoom(roomNum, guestName, nights);
                break;
            }
            case 4: {
                clearScreen();
                std::cout << "\n\033[36m================== [辦理退房結帳] ==================\033[0m\n";
                
                std::vector<std::shared_ptr<Room>> bookedRooms;
                for (const auto& r : manager.getAllRooms()) {
                    if (r->getIsBooked()) {
                        bookedRooms.push_back(r);
                    }
                }

                if (bookedRooms.empty()) {
                    std::cout << "\n\033[33m提示：目前沒有任何已入住/已預訂的客房！\033[0m\n";
                    break;
                }

                std::vector<std::string> checkoutOptions;
                for (const auto& r : bookedRooms) {
                    std::string typeChinese = "";
                    if (r->getRoomType() == "Single") typeChinese = "單人房";
                    else if (r->getRoomType() == "Double") typeChinese = "雙人房";
                    else if (r->getRoomType() == "Suite") typeChinese = "總統套房";

                    std::string opt = "房號 " + std::to_string(r->getRoomNumber()) + 
                                      " (" + typeChinese + ") - 住客: " + r->getGuestName();
                    checkoutOptions.push_back(opt);
                }
                checkoutOptions.push_back("返回主選單");

                int checkoutChoice = getMenuChoiceWithArrows("================== [辦理退房結帳 - 選擇退房客房] ==================", checkoutOptions);
                
                if (checkoutChoice == (int)checkoutOptions.size()) {
                    break;
                }

                int selectedRoomNum = bookedRooms[checkoutChoice - 1]->getRoomNumber();
                clearScreen();
                double cost = 0.0;
                if (manager.checkoutRoom(selectedRoomNum, cost)) {
                    std::cout << "\033[32m退房手續完成！房間已釋放為空房。\033[0m\n";
                }
                break;
            }
            case 5: {
                clearScreen();
                manager.showStatistics();
                break;
            }
            case 6: {
                clearScreen();
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
