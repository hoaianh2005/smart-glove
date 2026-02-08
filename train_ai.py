import pandas as pd
from sklearn.ensemble import RandomForestClassifier
from sklearn.preprocessing import LabelEncoder
from micromlgen import port
import os

# --- CẤU HÌNH ---
DATA_FILE = 'gesture_data.csv'
OUTPUT_HEADER = '../main/model.h'

print(f"--- BẮT ĐẦU HUẤN LUYỆN AI ---")

# 1. ĐỌC DỮ LIỆU
if not os.path.exists(DATA_FILE):
    print(f"LỖI: Không tìm thấy file {DATA_FILE}")
    print("Hãy đảm bảo bạn đã tạo file csv trong cùng thư mục này!")
    exit()

print("Đang đọc dữ liệu...")
data = pd.read_csv(DATA_FILE)

# Lấy 6 cột đầu làm đặc trưng (Flex1, Flex2, Flex3, Ax, Ay, Az)
X = data.iloc[:, :6].values
# Lấy cột cuối làm nhãn (Label)
y_raw = data.iloc[:, 6].values

# 2. MÃ HÓA NHÃN (Chuyển chữ thành số)
le = LabelEncoder()
y = le.fit_transform(y_raw)

print("\n" + "="*40)
print("  BẢNG MÃ HÓA (GHI NHỚ ĐỂ SỬA CODE C)")
print("="*40)
for i, label in enumerate(le.classes_):
    print(f"  Class {i}  ===>  {label}")
print("="*40 + "\n")

# 3. HUẤN LUYỆN MODEL
print("Đang huấn luyện (Random Forest)...")
# Dùng 20 cây quyết định, độ sâu tối đa 10 (để nhẹ cho ESP32)
model = RandomForestClassifier(n_estimators=20, max_depth=10, random_state=42)
model.fit(X, y)
print(f"Độ chính xác trên tập train: {model.score(X, y) * 100:.2f}%")

# 4. XUẤT RA CODE C (PORTING)
print("Đang chuyển đổi sang C...")
c_code = port(model)

# 5. LƯU VÀO FOLDER MAIN CỦA ESP32
try:
    with open(OUTPUT_HEADER, 'w') as f:
        f.write(c_code)
    print(f"THÀNH CÔNG! File đã được lưu tại: {os.path.abspath(OUTPUT_HEADER)}")
    print("Bây giờ hãy nạp code cho ESP32 (idf.py flash monitor).")
except Exception as e:
    print(f"LỖI KHI LƯU FILE: {e}")