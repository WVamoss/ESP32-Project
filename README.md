# **ESP32-Project**

---

# 🟦 PENJELASAN LENGKAP SISTEM ANTRIAN ESP32-S3

Sistem ini mensimulasikan **mesin antrian loket** menggunakan ESP32-S3 dengan komponen: OLED, encoder (putar + tombol), buzzer, dan tiga LED indikator.

Fungsi utama sistem:

* Membuka / menutup loket.
* Mengambil nomor antrian.
* Memanggil nomor antrian.
* Menampilkan status dan nomor di layar.

Seluruh proses berjalan otomatis berdasarkan interaksi pengguna melalui **encoder putar** dan **tombol encoder**.

---

# 1️⃣ **KOMPONEN DAN FUNGSINYA**

## ✔ Encoder Putar (CLK, DT)

Digunakan untuk **mengubah status loket** antara:

* **BUKA**
* **TUTUP**

Setiap kali encoder diputar → status akan di-*toggle* (berubah dari buka ke tutup atau sebaliknya), dan buzzer berbunyi singkat sebagai notifikasi.

---

## ✔ Tombol Encoder (SW)

Berfungsi sebagai **tombol pemanggilan nomor**.

Jika ditekan:

* Bila **loket TUTUP** → buzzer bunyi *error* (nada rendah).
* Bila **loket BUKA**:

  * Nomor antrian bertambah (`lastNumber++`)
  * Nomor yang dipanggil diset (`currentNumber = lastNumber`)
  * Sistem menjalankan animasi pemanggilan (LED & buzzer).

---

## ✔ OLED Display (SDA 38, SCL 39)

Menampilkan:

* Status Loket (BUKA / TUTUP)
* Nomor Sekarang: *currentNumber*
* Nomor Terakhir: *lastNumber*

Display diperbarui **setiap loop**.

---

## ✔ LED Indikator

### 🟢 **LED Hijau**

Menunjukkan **loket BUKA**.

### 🔴 **LED Merah**

Menunjukkan **loket TUTUP**.

### 🟡 **LED Kuning**

Indikator **proses pemanggilan nomor**.

👉 LED kuning **hanya** menyala saat sistem sedang memanggil nomor.
Dalam keadaan lain → **mati**.

(Detail penjelasan LED kuning ada di bagian 4.)

---

## ✔ LED Pintu (Pengganti Servo)

LED pada pin 15 berfungsi untuk mensimulasikan pergerakan pintu loket.

Nyala–mati cepat saat pemanggilan nomor.

---

## ✔ Buzzer

Bunyi dalam dua kondisi:

1. **Loket dibuka/diclose** (bunyi pendek)
2. **Nomor dipanggil** (bunyi cepat beberapa kali)
3. **Loket tutup tapi tombol ditekan** → bunyi error (nada rendah)

---

# 2️⃣ **ALUR KERJA SISTEM**

## A. **Mengubah Status Loket (Putar Encoder)**

Ketika encoder diputar:

* Posisi encoder berubah → status *loketBuka* di-*toggle*
* Bila loket berubah menjadi **BUKA**:

  * LED Hijau menyala
  * LED Merah mati
  * Buzzer beep
* Bila loket berubah menjadi **TUTUP**:

  * LED Merah menyala
  * LED Hijau mati
  * Buzzer beep
  * LED kuning selalu mati

OLED langsung memperbarui tampilan status.

---

## B. **Memanggil Nomor Antrian (Tekan Tombol SW)**

Ketika tombol ditekan:

### 🔴 Jika loket TUTUP:

* Tidak boleh memanggil nomor
* Buzzer bunyi *500 Hz* selama 200ms
* OLED tetap menampilkan “TUTUP”

### 🟢 Jika loket BUKA:

1. Tambah nomor antrian (`lastNumber++`)
2. Set nomor yang dipanggil (`currentNumber = lastNumber`)
3. Panggil animasi:

   * LED kuning berkedip cepat
   * LED pintu berkedip cepat
   * buzzer bunyi pendek beberapa kali
4. OLED menampilkan:

   ```
   Loket: BUKA
   Now: <currentNumber>
   Last: <lastNumber>
   ```

---

# 3️⃣ **ANIMASI PEMANGGILAN**

Fungsi:

```cpp
panggilNomor();
```

melakukan animasi berikut sebanyak 4 kali:

1. LED Kuning → ON
2. LED Pintu → ON
3. Buzzer → bunyi 1500 Hz singkat
4. Delay 120 ms
5. Kedua LED mati
6. Delay 120 ms

Animasi ini memberi efek visual & suara yang jelas bahwa nomor sedang dipanggil.

---

# 4️⃣ 🟡 **FUNGSI LENGKAP LED KUNING (Versi Terbaru)**

LED kuning sekarang berfungsi sebagai:

> **INDIKATOR BAHWA SISTEM SEDANG MEMANGGIL NOMOR ANTRIAN**

Bukan lagi untuk antrian pending.
Fungsinya fokus pada *calling indicator*.

### LED Kuning menyala dan berkedip hanya saat:

* Tombol ditekan → nomor dipanggil
* Sistem mengeksekusi animasi panggilan

### LED Kuning mati ketika:

* Loket tutup
* Loket buka tetapi belum ada panggilan
* Animasi panggilan selesai

Ini memberikan efek yang lebih realistis seperti mesin antrian di rumah sakit/kantor pelayanan.

---

# 5️⃣ **ALUR STATUS LED**

| Kondisi Sistem    | Hijau | Kuning    | Merah |
| ----------------- | ----- | --------- | ----- |
| Loket TUTUP       | OFF   | OFF       | ON    |
| Loket BUKA (idle) | ON    | OFF       | OFF   |
| Nomor dipanggil   | ON    | **Blink** | OFF   |
| Panggilan selesai | ON    | OFF       | OFF   |

---

# 6️⃣ **RINGKASAN CONCISE (SIAP UNTUK LAPORAN)**

* Encoder putar → ganti status loket (BUKA/TUTUP).
* Encoder button → memanggil nomor baru.
* Tambah nomor dilakukan otomatis (`lastNumber++`).
* OLED menampilkan status dan nomor.
* LED hijau = buka, LED merah = tutup.
* LED kuning = indikator panggilan nomor (blink).
* LED pintu = simulasi servo (blink saat panggilan).
* Buzzer memberi umpan balik suara.

---


