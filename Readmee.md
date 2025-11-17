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
