/* ====== GANTI NILAI DI SINI DENGAN HASIL SENSOR KAMU ======
   Ganti angka suhu, kelembapan udara, dan kelembapan tanah
   sesuai data dari Arduino/monitor kamu.
   Gak pakai koneksi internet/ThingSpeak, murni buat nampilin hasil aja.
*/
const suhu = 27.5;         // ganti sesuai suhu (°C)
const kelembapan = 65;     // ganti sesuai kelembapan udara (%)
const soil = 45;           // ganti sesuai kelembapan tanah (%)

const tempValue = document.getElementById('tempValue');
const humValue  = document.getElementById('humValue');
const soilValue = document.getElementById('soilValue');
const soilFill  = document.getElementById('soilFill');
const tempNote  = document.getElementById('tempNote');
const humNote   = document.getElementById('humNote');
const soilNote  = document.getElementById('soilNote');
const bubble    = document.getElementById('bubble');
const fanStatus = document.getElementById('fanStatus');
const pumpStatus= document.getElementById('pumpStatus');

function tampilkan(suhu, kelembapan, soil) {
  const fanOn  = (suhu > 24 && kelembapan > 70);
  const pumpOn = (soil < 30);

  tempValue.textContent = suhu.toFixed(1);
  humValue.textContent  = kelembapan.toFixed(1);
  soilValue.textContent = soil.toFixed(1);
  soilFill.style.width = Math.min(Math.max(soil, 0), 100) + '%';

  tempNote.textContent = suhu > 30 ? 'panas' : suhu > 24 ? 'anget' : suhu < 20 ? 'adem' : 'pas';
  humNote.textContent  = kelembapan > 70 ? 'lembap' : kelembapan < 40 ? 'kering' : 'nyaman';
  soilNote.textContent = soil < 30 ? 'kering' : soil < 60 ? 'lumayan lembap' : 'basah seger';

  fanStatus.textContent  = fanOn ? 'ON 💨' : 'OFF';
  pumpStatus.textContent = pumpOn ? 'ON 💦' : 'OFF';

  if (fanOn && pumpOn) bubble.textContent = 'lagi gerah & tanah kering, kipas & pompa nyala bareng~';
  else if (fanOn) bubble.textContent = 'lagi gerah, kipas nyala biar adem~';
  else if (pumpOn) bubble.textContent = 'tanahnya kering, pompa lagi nyala~';
  else bubble.textContent = 'semua kondisi aman terkendali 🌈';
}

tampilkan(suhu, kelembapan, soil);
