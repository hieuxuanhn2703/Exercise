Xây dựng một sơ đồ mạch trên Proteus và lập trình, mạch gồm có:
- Sử dụng VĐK STM32 
- Giảo tiếp với matrix key 4x4
- Cảm biến nhiệt độ NTC
- Màn hình LCD 20x4
- Thẻ nhớ SD card
- Giao tiếp UART với máy tính
Hoạt động: 
- Chương trình sử dụng FreeRTOS
- Chương trình gồm 2 chế độ hoạt động là Running và config
- Trong chế độ Running:
+ Động cơ quay thuận với tốc độ s1 trong thời gian t1, sau đó đảo chiều quay với tốc độ s2 trong thời gian t2 (tốc độ quy đổi theo độ rộng xung PWM cũng đc ko cần đọc encoder, thời gian tính theo giây)
+ Cảm biến nhiệt độ NTC được đọc với chu kỳ 1s 1 lần
+ Ghi vào thẻ nhớ SD card thông tin dưới dạng file, mỗi lần khởi động mạch sẽ tạo 1 file mới, mỗi lần ghi các thông tin gồm có: thời gian t1, thời gian t2, tốc độ s1 tốc độ s2, nhiệt độ NTC. (sử dụng thư viện FATFS hoặt một thư viện nào đó hỗ trợ đọc ghi định dạng file cũng đc)
+ Hiển thị lên màn hình LCD các thông tin t1,t2,s1,s2,NTC
+ Gửi lên UART các thông tin tin t1,t2,s1,s2,NTC định dạng giống như ghi vào file
- Trong chế độ Config:
+ Nhấn 1 tổ hợp 2 phím (tự chọn) trên matrix key để vào màn hình nhập password
+ Nhập đúng password thì vào màn hình config
+ Màn config dạng menu có 6 mục, vào 4 mục để cài đặt các thông số t1 t2 s1 s2, 1 mục vào để thay đổi password, một mục để save các thông tin đã bị thay đổi ở các mục còn lại vào thẻ nhớ flash (thẻ nhớ flash để 1 file lưu config các thông số kia)
+ Trong màn hình config dùng 1 nút để di chuyển menu, 1 nút để enter, một nút exit, các nút số để nhập password và thông số
