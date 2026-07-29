/* ====== GANTI NILAI DI SINI DENGAN HASIL SENSOR KAMU ======
   Ganti angka suhu & kelembapan sesuai data dari Arduino/monitor kamu.
   Gak pakai koneksi internet/ThingSpeak, murni buat nampilin hasil aja.
*/
const suhu = 27.5;         // ganti sesuai suhu (°C)
const kelembapan = 65;     // ganti sesuai kelembapan udara (%)

const tempValue = document.getElementById('tempValue');
const humValue  = document.getElementById('humValue');
const tempNote  = document.getElementById('tempNote');
const humNote   = document.getElementById('humNote');
const bubble    = document.getElementById('bubble');
const fanStatus = document.getElementById('fanStatus');

function tampilkan(suhu, kelembapan) {
  const fanOn = (suhu > 24 && kelembapan > 70);

  tempValue.textContent = suhu.toFixed(1);
  humValue.textContent  = kelembapan.toFixed(1);

  tempNote.textContent = suhu > 30 ? 'panas' : suhu > 24 ? 'anget' : suhu < 20 ? 'adem' : 'pas';
  humNote.textContent  = kelembapan > 70 ? 'lembap' : kelembapan < 40 ? 'kering' : 'nyaman';

  fanStatus.textContent = fanOn ? 'ON 💨' : 'OFF';
  bubble.textContent = fanOn
    ? 'lagi gerah, kipas nyala biar adem~'
    : 'udaranya adem, semua aman terkendali 🌷';
}

tampilkan(suhu, kelembapan);
