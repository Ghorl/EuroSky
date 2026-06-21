const express = require('express');
const { execSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const app = express();

// Servir archivos estáticos
app.use(express.static('.'));                              // Carpeta frontend
app.use(express.static(path.join(__dirname, '..')));     // Carpeta padre (data/)
app.use(express.json());

// API para buscar rutas
app.post('/api/search-route', (req, res) => {
    const { origin, destination, maxHours, algorithm } = req.body;
    const algo = algorithm || 'dijkstra'; // Default: dijkstra
    
    console.log(`[Buscando ruta] ${origin} → ${destination} (${maxHours}h) [${algo}]`);
    
    const projectRoot = path.join(__dirname, '..');
    const cmd = `cd "${projectRoot}" && ./eurosky_test ${origin} ${destination} ${maxHours} ${algo}`;
    
    console.log(`[Ejecutando] ${cmd}`);
    
    try {
        execSync(cmd, { stdio: 'pipe' });
        console.log('[✓] Programa ejecutado');
    } catch (execErr) {
        console.error('[✗] Error ejecutando:', execErr.message);
        res.writeHead(500, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'No se pudo ejecutar el programa: ' + execErr.message }));
        return;
    }
    
    const jsonPath = path.join(projectRoot, 'resultados_ruta.json');
    console.log(`[Leyendo JSON] ${jsonPath}`);
    
    try {
        if (!fs.existsSync(jsonPath)) {
            throw new Error('El archivo resultados_ruta.json no se generó');
        }
        
        const jsonData = fs.readFileSync(jsonPath, 'utf8');
        console.log('[✓] JSON leído correctamente');
        
        res.writeHead(200, { 'Content-Type': 'application/json' });
        res.end(jsonData);
    } catch (err) {
        console.error('[✗] Error:', err.message);
        res.writeHead(500, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: err.message }));
    }
});

app.listen(8000, () => {
    console.log('✓ Servidor en http://localhost:8000');
});