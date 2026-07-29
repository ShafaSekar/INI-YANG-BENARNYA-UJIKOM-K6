/* ====== GANTI NILAI DI SINI DENGAN HASIL SENSOR KAMU ======
   Ganti angka kelembapan tanah sesuai data dari Arduino/monitor kamu.
   Gak pakai koneksi internet/ThingSpeak, murni buat nampilin hasil aja.
*/
const soil = 45; // ganti sesuai kelembapan tanah (%)

const soilValue  = document.getElementById('soilValue');
const soilFill   = document.getElementById('soilFill');
const soilNote   = document.getElementById('soilNote');
const bubble     = document.getElementById('bubble');
const pumpStatus = document.getElementById('pumpStatus');

function tampilkan(soil) {
  const pumpOn = soil < 30;

  soilValue.textContent = soil.toFixed(1);
  soilFill.style.width = Math.min(Math.max(soil, 0), 100) + '%';
  soilNote.textContent = soil < 30 ? 'kering' : soil < 60 ? 'lumayan lembap' : 'basah seger';

  pumpStatus.textContent = pumpOn ? 'ON 💦' : 'OFF';
  bubble.textContent = pumpOn
    ? 'tanahnya kering, pompa lagi nyala~'
    : 'tanahnya cukup lembap, aman terkendali 🌿';
}

tampilkan(soil);
