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

---

# 🟦 **ALUR LOGIKA KERJA SISTEM**

## **1. Saat ESP32 Menyala**

* LED merah menyala → loket dalam status **TUTUP**
* OLED menampilkan:

  ```
  Sistem Antrian
  Loket TUTUP
  ```

---

## **2. Mengubah Mode Loket (BUKA / TUTUP)**

* Putar encoder → nilai encoder berubah.
* Program mendeteksi perubahan, lalu:

### Jika sebelumnya TUTUP → jadi BUKA

* LED Hijau menyala
* LED Merah mati
* Buzzer bunyi singkat → *tanda loket dibuka*

### Jika sebelumnya BUKA → jadi TUTUP

* LED Merah menyala lagi
* LED Hijau & Kuning mati
* Buzzer bunyi

**OLED otomatis memperbarui:**

```
Loket: BUKA / TUTUP
Now: X
Last: Y
```

---

## **3. Tekan Tombol Encoder (SW) → *Ambil Nomor + Panggil Nomor***

Setiap kali tombol ditekan:

### **a) Jika LOKET TUTUP**

* Tidak boleh ambil nomor
* Buzzer bunyi *error tone*
* Angka tidak berubah

### **b) Jika LOKET BUKA**

Program menjalankan proses **panggil nomor**:

### (1) lastNumber bertambah

Misal sebelumnya 5:

```
lastNumber = 6
```

### (2) currentNumber mengikuti lastNumber

Karena sistem ini contoh sederhana, setiap kali tombol ditekan langsung dipanggil nomor:

```
currentNumber = lastNumber
```

### (3) LED Pintu berkedip

Ini menggantikan servo:

* LED 15 nyala–mati cepat 4 kali
* Buzzer bunyi setiap kedipan

Efeknya menyerupai:

```
"Nomor 6 silakan ke loket"
```

### (4) OLED update:

```
Now: 6
Last: 6
```

### (5) LED Kuning

LED kuning dapat menyala jika:

```
Ada nomor menunggu
(lastNumber > currentNumber)
```

Namun karena sistem ini langsung memanggil nomor, LED kuning biasanya mati.

---

# 🟧 **RANGKUMAN TIAP BAGIAN**

## **A. Encoder → Mode Loket**

* Diputar → mengubah status BUKA / TUTUP
* Buzzer bunyi
* OLED & LED status ikut berubah

---

## **B. Tombol Encoder (SW) → Ambil Nomor**

* Jika TUTUP → buzzer error
* Jika BUKA:

  * Tambah nomor
  * Panggil nomor (simulasi)
  * Update OLED
  * LED pintu berkedip (simulasi servo)
  * Buzzer bunyi

---

## **C. OLED Menampilkan Info**

OLED selalu menampilkan:

1. Status: BUKA / TUTUP
2. Nomor yang sedang dipanggil (`currentNumber`)
3. Nomor terakhir (`lastNumber`)

Diupdate setiap loop.

---

## **D. LED Status Loket**

* **Merah = Loket Tutup**
* **Hijau = Loket Buka**
* **Kuning = Ada antrian menunggu** (opsional)

---

## **E. LED Pintu Loket (GPIO 15)**

Ini pengganti servo.

* Berkedip cepat = pintu terbuka
* Mati = pintu normal

---

# 🟩 **ILUSTRASI CERITA / SCENARIO**

### **1. Petugas membuka loket**

Putar encoder → Loket BUKA
LED Hijau menyala, OLED update.

---

### **2. Pengunjung menekan tombol**

SW ditekan → mesin ambil nomor
Misalnya:

* lastNumber = 1
* currentNumber = 1
* LED pintu berkedip (panggilan)
* Buzzer bunyi

---

### **3. Pengunjung berikutnya datang**

SW ditekan lagi:

* lastNumber = 2
* currentNumber = 2
* LED pintu berkedip lagi

---

### **4. Loket ditutup lagi**

Putar encoder → Loket TUTUP
Semua efek dimatikan.

---

# 🟦 Kalau mau, saya bisa tambahkan:

* antrian dengan queue beneran (queue RTOS)
* tombol ambil nomor terpisah
* suara buzzer kaya suara counter asli
* mode prioritas (VIP / reguler)
* tampilan OLED lebih keren
* multi-loket (loket 1,2,3)
�
