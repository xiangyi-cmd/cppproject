# 智慧型訂房與客房管理系統 (C++ Terminal Hotel Booking System)

本專案是一個用 C++ 撰寫的**智慧型訂房與客房管理系統**，為期末專題的完整實作。專案採用 **OpenSpec 規格驅動開發 (Spec-Driven Development)** 流程，結合物件導向核心設計，提供視覺化的終端機操作介面。

---

## 1. 系統功能與設計特色

### 1.1 物件導向類別繼承與多型 (OOP Inheritance & Polymorphism)
系統設計了豐富的客房層級結構，以 `Room` 為基底類別，擴展出三種不同計費與服務特性的房型：
* **`Room` (基底類別)**：定義客房的核心屬性（房號、基本價格、預訂狀態、住客姓名、天數）與虛擬解構子，並宣告三大關鍵純虛擬函式，供衍生類別實作以展現多型特徵：
  * `virtual double calculateTotalCost(int nights) const = 0`：依據房型計算總費用（多型）。
  * `virtual void displayRoomDetails() const = 0`：顯示房型專屬詳細說明（多型）。
  * `virtual std::string getRoomType() const = 0`：取得房型標籤。
* **`SingleRoom` (單人房，衍生類別)**：標準基本計費。
* **`DoubleRoom` (雙人房，衍生類別)**：每日房價加計早餐費與服務費。
* **`Suite` (總統套房，衍生類別)**：加收一次性的 VIP 貴賓室使用費，並加計 10% 奢侈稅。

### 1.2 STL 容器與演算法 (STL Containers & Algorithms)
* **記憶體與多型管理**：使用 `std::vector<std::shared_ptr<Room>>` 儲存多型物件，以安全且智慧的指標（Smart Pointers）管理記憶體，防止記憶體洩漏。
* **歷史訂單**：使用 `std::vector<BookingRecord>` 記錄訂單歷史。
* **搜尋 (Search)**：使用 STL 演算法 `std::find_if` 搭配 Lambda 運算式，實現房號的快速精確尋找。
* **排序 (Sort)**：使用 STL 演算法 `std::sort` 搭配 Lambda，提供「房號排序」與「房價低至高排序」兩種模式。

### 1.3 持久化 CSV 檔案讀寫 (File I/O)
* 系統啟動時，會從檔案載入資料：
  * `data/rooms.csv`：載入所有客房最新的狀態，若上次有關閉前被預訂的客房，系統能自動恢復住客與天數狀態。
  * `data/bookings.csv`：載入歷史所有成功的預訂交易明細。
* 系統正常退出時，會自動回寫並覆蓋上述檔案，確保資料永不遺失。

### 1.4 彩色視覺化終端機介面與防呆機制 (Visual CLI & Robust Validation)
* **狀態視覺化 (ANSI Colors)**：客房狀態牆以 ANSI 顏色呈現，**`[綠色]`** 代表空閒，**`[紅色]`** 代表已入住，清晰直觀。
* **輸入防呆驗證**：利用 `std::cin.clear()` 與 `std::cin.ignore()`，對使用者的選單選擇、入住天數、房號、金額預算等進行嚴格的型態與範圍限制，避免使用者輸入錯誤字元（如英文字母）導致程式崩潰或無窮迴圈。

---

## 2. 專案目錄結構

```text
cpp_hotel_booking/
├── compile.bat         # Windows 一鍵編譯指令檔
├── run.bat             # Windows 一鍵編譯並執行指令檔
├── README.md           # 本說明文件
├── data/
│   ├── bookings.csv    # 歷史訂單紀錄檔
│   └── rooms.csv       # 客房狀態檔
├── include/
│   ├── HotelManager.h  # 飯店管理核心宣告
│   └── Room.h          # 客房基底與衍生類別宣告
├── src/
│   ├── HotelManager.cpp# 飯店管理核心實作
│   ├── Room.cpp        # 客房類別實作
│   └── main.cpp        # 系統主入口與 CLI 選單迴圈
└── openspec/
    └── specs/
        └── system_spec.md # OpenSpec 系統規格書 (Source of Truth)
```

---

## 3. 編譯與執行說明

### 3.1 環境需求
* 安裝有 **GCC / g++**（支援 C++17 標準以上）的編譯環境（例如 MinGW-w64）。

### 3.2 一鍵編譯與執行 (Windows)
專案根目錄已提供便利的批次處理指令檔：
1. **編譯專案**：雙擊 `compile.bat`，或在終端機執行 `.\compile.bat`，系統將自動編譯並在根目錄產生 `hotel_system.exe`。
2. **執行專案**：雙擊 `run.bat`，或在終端機執行 `.\run.bat`，系統會自動檢查並編譯（若尚未編譯），隨後啟動訂房系統。

### 3.3 手動指令編譯 (Manual Compilation)
若需手動編譯，請於根目錄下在終端機（如 PowerShell 或 CMD）輸入：
```bash
# 請確保 C:\mingw64\bin 在環境變數中，或使用實體路徑：
C:\mingw64\bin\g++.exe -std=c++17 -Iinclude src/Room.cpp src/HotelManager.cpp src/main.cpp -o hotel_system.exe
```

---

## 4. OpenSpec 規格驅動開發流程 (Spec-Driven Development)
本專案嚴格遵循 OpenSpec 開發規範：
1. **規格定義**：在動手編寫任何程式碼前，先於 `openspec/specs/system_spec.md` 中詳細規劃類別 UML 圖、CSV 資料庫欄位、計費多型演算法與選單流程。
2. **規格實作**：根據規格書中定義的 API 與資料結構，依次編寫標頭檔、實作檔與主程式，確保程式碼與規劃 100% 一致。
3. **驗證測試**：依據規格書中的手動測試清單進行驗證，確保多型計費、防呆輸入與 CSV 讀寫等功能全部通過。
