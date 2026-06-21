# ✈️ EuroSky Connect - Route Optimizer

Un sistema profesional de **optimización de rutas aéreas** que utiliza el algoritmo **Dijkstra** para encontrar la ruta más económica entre aeropuertos europeos, con visualización en **Google Maps** y panel interactivo de costos.

---

## 📋 Características

✅ **Optimización de rutas aéreas**
- Algoritmo Dijkstra con priority queue
- Selección automática del avión más barato por ruta
- Restricción de tiempo máximo de vuelo (8 horas configurables)
- Red de 38 aeropuertos europeos con 100+ rutas

✅ **Modelo de costos detallado**
- Combustible (basado en consumo y precio del litro)
- Tripulación (costo por hora de vuelo)
- Mantenimiento (costo por hora de vuelo)
- Tasas de aterrizaje (por aeropuerto)
- Servicios de handling (por aeropuerto)

✅ **Interfaz web moderna**
- Frontend HTML/CSS/JS responsivo
- Integración Google Maps API
- Selección dinámica de origen/destino
- Panel de costos desglosado en tiempo real
- Visualización de rutas en mapa

✅ **Arquitectura escalable**
- Backend C++ profesional
- Servidor Node.js como middleware
- API REST `/api/search-route`
- Rutas cargadas desde JSON (sin hardcode)

---

## 🏗️ Arquitectura

```
┌─────────────────────────────────────────────────────────┐
│                    Frontend (Web)                        │
│  HTML/CSS/JS + Google Maps API                          │
│  - Selección origen/destino                             │
│  - Visualización en mapa                                │
│  - Panel de costos                                      │
└────────────────────┬────────────────────────────────────┘
                     │ HTTP POST /api/search-route
                     ↓
┌─────────────────────────────────────────────────────────┐
│              Servidor Node.js (Middleware)               │
│  - Recibe parámetros (origen, destino, max_horas)      │
│  - Ejecuta programa C++                                 │
│  - Lee JSON generado                                    │
│  - Retorna resultado al frontend                        │
└────────────────────┬────────────────────────────────────┘
                     │ spawn./eurosky_test
                     ↓
┌─────────────────────────────────────────────────────────┐
│           Backend C++ (Dijkstra Optimizer)               │
│  - Carga 38 aeropuertos (airports.json)                │
│  - Carga 100+ rutas (routes.json)                       │
│  - Asigna aviones a aeropuertos                         │
│  - Ejecuta Dijkstra                                     │
│  - Calcula costos desglosados                           │
│  - Genera resultados_ruta.json                          │
└─────────────────────────────────────────────────────────┘
```

---

## 🛠️ Requisitos

- **C++17** o superior
- **Node.js v20+** con npm
- **Google Maps API Key** (gratis)
- **Linux/macOS/Windows** con make

### Dependencias C++
- nlohmann/json (incluido en `include/`)

### Dependencias Node.js
- Express.js (instalado vía npm)

---

## 📥 Instalación

### 1. Clonar/Descargar el proyecto

```bash
cd EuroSky
```

### 2. Compilar backend C++

```bash
make clean
make build
```

Debería generar el ejecutable `eurosky_test`.

### 3. Instalar dependencias Node.js

```bash
cd frontend
npm install express
cd ..
```

### 4. Configurar Google Maps API

1. Obtener API Key: https://console.cloud.google.com/
2. Abrir `frontend/index.html`
3. Reemplazar `TU_API_KEY_AQUI` con tu clave:

```html
<script src="https://maps.googleapis.com/maps/api/js?key=TU_API_KEY_AQUI"></script>
```

---

## 🚀 Uso

### Opción A: Ejecución manual + Web

**Terminal 1: Ejecutar servidor Node.js**
```bash
cd frontend
node server.js
```

**Terminal 2: Abrir navegador**
```
http://localhost:8000
```

Selecciona origen/destino y presiona "Buscar Ruta Óptima".

### Opción B: Línea de comandos (C++ directo)

```bash
./eurosky_test <origen> <destino> <max_horas>

# Ejemplos:
./eurosky_test MAD AMS 8.0
./eurosky_test BCN LHR 8.0
./eurosky_test VIE DUB 8.0
```

Genera `resultados_ruta.json` con los resultados.

---

## 📂 Estructura del Proyecto

```
EuroSky/
├── include/                    # Headers C++
│   ├── Aircraft.h
│   ├── Airport.h
│   ├── Route.h
│   ├── Graph.h
│   ├── CostModel.h
│   ├── Dijkstra.h
│   └── json.hpp               # nlohmann/json
│
├── src/                        # Implementación C++
│   ├── Aircraft.cpp
│   ├── Airport.cpp
│   ├── Route.cpp
│   ├── Graph.cpp
│   ├── CostModel.cpp
│   ├── Dijkstra.cpp
│   └── main.cpp
│
├── data/                       # Datos JSON
│   ├── airports.json          # 38 aeropuertos europeos
│   └── routes.json            # 100+ rutas aéreas
│
├── frontend/                   # Frontend web
│   ├── index.html             # Interfaz web
│   ├── server.js              # Servidor Node.js
│   ├── package.json
│   └── node_modules/
│
├── Makefile                    # Build system
├── eurosky_test               # Ejecutable (compilado)
├── resultados_ruta.json       # Output (generado)
└── README.md                  # Este archivo
```

---

## 🔑 Conceptos Clave

### Algoritmo Dijkstra Modificado

El proyecto implementa una versión mejorada de Dijkstra que:

1. **Selecciona automáticamente el avión más barato** por cada ruta
2. **Valida restricciones**:
   - Rango del avión (¿puede volar esa distancia?)
   - Tiempo acumulado (¿no excede 8 horas?)
3. **Relajación de costos**: Solo actualiza si encuentra camino más barato
4. **Priority Queue**: Eficiencia O(E log V)

### Modelo de Costos

```
COSTO_TOTAL = Combustible + Tripulación + Mantenimiento + Aterrizaje + Handling

Combustible    = (distancia/velocidad) × consumo/h × precio_litro
Tripulación    = (distancia/velocidad) × costo_tripulación/h
Mantenimiento  = (distancia/velocidad) × costo_mantenimiento/h
Aterrizaje     = tarifa_destino
Handling       = servicio_destino
```

### Aviones Disponibles

1. **Airbus A320** (180 pax): Equilibrio costo-capacidad
2. **Boeing 787** (242 pax): Largo alcance, consumo alto
3. **ATR 72-600** (78 pax): Corta distancia, consumo bajo

---

## 📊 Resultados Ejemplo

**Búsqueda: MAD → AMS (máx 8 horas)**

```
Ruta óptima: MAD → AMS
Costo total: €12,234.60
Tiempo total: 3.46 horas
Avión seleccionado: ATR 72-600

Desglose:
  Combustible:    €2,180.77
  Tripulación:    €2,769.23
  Mantenimiento:  €1,384.62
  Aterrizaje:     €4,500.00
  Servicios:      €1,400.00
```

---

## 🧪 Testing

### Pruebas recomendadas

```bash
# Ruta corta (España)
./eurosky_test MAD BCN 8.0

# Ruta media (Europa)
./eurosky_test MAD AMS 8.0

# Ruta larga (Europa)
./eurosky_test MAD VIE 8.0

# Sin ruta disponible
./eurosky_test MAD ARN 2.0  # Insuficiente tiempo
```

### Validaciones

✅ El programa valida:
- Aeropuertos existentes
- Tiempo máximo de vuelo
- Rango de aviones
- Rutas disponibles

---

## 🎯 Mejoras Futuras

- [ ] Soporte para rutas con escalas (multi-hop)
- [ ] API REST con autenticación
- [ ] Base de datos (PostgreSQL/MongoDB)
- [ ] Historial de búsquedas
- [ ] Exportar resultados (PDF/CSV)
- [ ] Dashboard administrativo
- [ ] Cálculo de distancias en tiempo real (Google Maps Distance Matrix API)
- [ ] Actualización dinámica de precios de combustible

---

## 👨‍💻 Tecnologías Utilizadas

**Backend**
- C++17 (Algoritmos, estructuras de datos)
- Dijkstra (Optimización de rutas)
- nlohmann/json (Parsing JSON)

**Frontend**
- HTML5 / CSS3 (Interfaz)
- JavaScript ES6+ (Lógica)
- Google Maps API (Visualización)

**Infraestructura**
- Node.js (Servidor middleware)
- Express.js (API REST)
- Make (Build system)

---

## 📝 Notas Importantes

1. **API Key Google Maps**: Necesaria para visualizar rutas en el mapa
2. **Rutas dinámicas**: Se cargan desde `data/routes.json` (sin recompilar)
3. **Distancias realistas**: Basadas en coordenadas GPS de aeropuertos
4. **Escalabilidad**: Soporta fácilmente más aeropuertos y rutas

---

## 📄 Licencia

Proyecto académico - Universidad

---

## ✉️ Autor

Proyecto desarrollado como trabajo final de programación avanzada.

**Características destacadas:**
- Algoritmo profesional (Dijkstra con priority queue)
- Arquitectura escalable (datos desde JSON)
- Interfaz web moderna (Google Maps)
- Backend robusto (C++ 17)
