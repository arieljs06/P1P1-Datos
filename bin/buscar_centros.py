import urllib.parse
import requests
import sys

def buscar_centros(direccion):
    direccion_encoded = urllib.parse.quote(direccion)
    
    headers = {
        'User-Agent': 'mi-turnero-estudiantil/1.0 (correo@ejemplo.com)'
    }

    url = f"https://nominatim.openstreetmap.org/search?q={direccion_encoded}&format=json&countrycodes=ec&limit=1"
    resp = requests.get(url, headers=headers)

    if resp.status_code != 200:
        print("Error al consultar la API de Nominatim.")
        return 1  # Código de error

    try:
        data = resp.json()
        if not data:
            print("No se encontró la ubicación. Verifique la dirección.")
            return 1  # Código de error
    except Exception as e:
        print("Error al procesar la respuesta de Nominatim:", e)
        return 1  # Código de error

    lat, lon = data[0]["lat"], data[0]["lon"]

    query = f"""
    [out:json];
    (
      node["amenity"="hospital"](around:5000,{lat},{lon});
      node["amenity"="clinic"](around:5000,{lat},{lon});
      node["healthcare"="centre"](around:5000,{lat},{lon});
    );
    out;
    """

    resp = requests.get("https://overpass-api.de/api/interpreter", params={'data': query})
    result = resp.json()

    print(f"\nCentros médicos cerca de: {direccion}\n")
    if not result["elements"]:
        print("No se encontraron centros médicos cercanos.")
    else:
        for elem in result["elements"][:5]:
            name = elem.get("tags", {}).get("name", "Nombre no disponible")
            print(" -", name)
    
    return 0  # Éxito

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Uso: python buscar_centros.py \"dirección\"")
        sys.exit(1)
    
    direccion = sys.argv[1]
    sys.exit(buscar_centros(direccion))