# **ESP32-Project**

# 🟩 **GAMBARAN UMUM**

Sistem ini mensimulasikan **mesin antrian loket**, tapi disederhanakan supaya bisa jalan di Wokwi **tanpa servo** → pintu loket diganti LED.

Komponen yang digunakan:

* **Encoder** = untuk mengubah status BUKA / TUTUP
* **Button (SW encoder)** = sebagai tombol "NEXT / AMBIL NOMOR"
* **OLED** = menampilkan nomor antrian
* **LED Hijau/Kuning/Merah** = status loket
* **LED Pintu (GPIO 15 Blue)** = simulasi pembukaan pintu
* **Buzzer** = bunyi ketika memanggil nomor


(Multicore, Semaphore, Mutex) 

---

### **1. Multicore (ESP32 Dual-Core Processing)**

Dalam project ini, konsep **multicore** diimplementasikan dengan memisahkan beban kerja antara **Core 0** dan **Core 1** supaya sistem lebih responsif dan tidak terjadi bottleneck. Misalnya, Core 0 fokus menjalankan pembacaan encoder, tombol input, dan logika antrian, sementara Core 1 meng-handle output seperti LED, buzzer, dan simulasi servo (LED pintu). Pemisahan ini membuat proses input tidak terganggu oleh delay pada output, sehingga sistem tetap responsif meskipun beberapa bagian program membutuhkan waktu tunggu. Dengan pembagian tugas ini, performa perangkat meningkat dan seluruh fitur dapat berjalan secara paralel tanpa saling menahan eksekusi.

---

### **2. Semaphore (Sinkronisasi Antar-Task)**

Semaphore digunakan sebagai alat sinkronisasi agar task yang berjalan di core berbeda tidak saling mendahului atau mengambil data sebelum siap. Contohnya, task pemanggilan nomor hanya boleh dijalankan setelah task pembacaan encoder mengirim sinyal siap menggunakan semaphore. Dengan begitu, sistem dapat memastikan bahwa proses pemanggilan nomor tidak terjadi tanpa input valid dari encoder atau tombol. Implementasi ini mencegah racing antar-task, mengontrol alur eksekusi, dan memastikan setiap proses dijalankan pada waktunya tanpa konflik.

---

### **3. Mutex (Pengamanan Variabel Bersama)**

Mutex digunakan untuk **melindungi variabel yang digunakan bersama** seperti `currentNumber`, `lastNumber`, dan status `loketBuka`. Karena variabel ini dibaca dan diubah oleh task yang berjalan di dua core berbeda, mutex memastikan hanya satu task yang dapat mengakses atau mengubah datanya pada satu waktu. Ini mencegah kondisi balapan (race condition) yang dapat menyebabkan nomor antrian berubah tidak sesuai urutan atau status loket menjadi tidak konsisten. Dengan mutex, data selalu aman, stabil, dan tidak rusak akibat akses bersamaan.



