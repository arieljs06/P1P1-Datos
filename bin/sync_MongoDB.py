#!/usr/bin/env python3
from pymongo import MongoClient
import sys
from datetime import datetime

# Configuración común
MONGO_URI = "mongodb://localhost:27017/"
DB_NAME = "citasMedicas"

def get_db_connection():
    """Establece conexión con MongoDB"""
    try:
        client = MongoClient(MONGO_URI, serverSelectionTimeoutMS=5000)
        client.admin.command('ping')  # Verifica conexión
        return client[DB_NAME]
    except Exception as e:
        print(f"Error de conexión a MongoDB: {str(e)}", file=sys.stderr)
        sys.exit(1)

def sincronizar_pacientes(ruta_archivo):
    """Sincroniza pacientes desde MongoDB a archivo"""
    try:
        db = get_db_connection()
        
        # Obtener pacientes ordenados por apellido y nombre
        pacientes = db['pacientes'].find().sort([("apellido", 1), ("nombre", 1)])

        # Escribir archivo con formato específico
        with open(ruta_archivo, 'w', encoding='utf-8') as f:
            for paciente in pacientes:
                f.write(f"Nombre: {paciente.get('nombre', '')}\n")
                f.write(f"Apellido: {paciente.get('apellido', '')}\n")
                f.write(f"Cédula: {paciente.get('cedula', '')}\n")
                f.write(f"Dirección: {paciente.get('direccion', '')}\n")
                f.write(f"Correo: {paciente.get('correo', '')}\n")
                f.write(f"Telefono: {paciente.get('telefono', '')}\n")
                f.write(f"Sexo: {paciente.get('sexo', '')}\n")
                f.write("------------------------------\n")

        print(f"Pacientes sincronizados en: {ruta_archivo}")
        return True

    except Exception as e:
        print(f"Error sincronizando pacientes: {str(e)}", file=sys.stderr)
        return False

def sincronizar_turnos(ruta_archivo):
    """Sincroniza turnos desde MongoDB a archivo"""
    try:
        db = get_db_connection()
        
        # Obtener turnos ordenados por fecha
        turnos = db['turnos'].find().sort([
            ("fecha_hora.anio", 1),
            ("fecha_hora.mes", 1),
            ("fecha_hora.dia", 1)
        ])

        # Escribir archivo con formato específico
        with open(ruta_archivo, 'w', encoding='utf-8') as f:
            for turno in turnos:
                paciente = turno.get('paciente', {})
                fh = turno.get('fecha_hora', {})
                
                # Datos del paciente
                f.write(f"Nombre: {paciente.get('nombre', '')}\n")
                f.write(f"Apellido: {paciente.get('apellido', '')}\n")
                f.write(f"Cédula: {paciente.get('cedula', '')}\n")
                f.write(f"Dirección: {paciente.get('direccion', '')}\n")
                f.write(f"Correo: {paciente.get('correo', '')}\n")
                f.write(f"Telefono: {paciente.get('telefono', '')}\n")
                f.write(f"Sexo: {paciente.get('sexo', '')}\n")
                
                # Datos específicos del turno
                f.write(f"Provincia: {turno.get('provincia', '')}\n")
                f.write(f"Ciudad: {turno.get('ciudad', '')}\n")
                f.write(f"Fecha y Hora {fh.get('dia', '')}/{fh.get('mes', '')}/{fh.get('anio', '')} -> {fh.get('hora', '')}:{fh.get('minuto', '')}\n")
                f.write("------------------------------\n")

        print(f"Turnos sincronizados en: {ruta_archivo}")
        return True

    except Exception as e:
        print(f"Error sincronizando turnos: {str(e)}", file=sys.stderr)
        return False

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Uso: python servidor_intermedio.py <tipo> <ruta_archivo>", file=sys.stderr)
        print("Tipos válidos: pacientes, turnos", file=sys.stderr)
        sys.exit(1)
    
    tipo = sys.argv[1].lower()
    ruta_archivo = sys.argv[2]
    
    if tipo == "pacientes":
        success = sincronizar_pacientes(ruta_archivo)
    elif tipo == "turnos":
        success = sincronizar_turnos(ruta_archivo)
    else:
        print(f"Error: Tipo no válido '{tipo}'", file=sys.stderr)
        sys.exit(1)
    
    sys.exit(0 if success else 1)