#!/usr/bin/env python3
from pymongo import MongoClient
import sys
from datetime import datetime

# Configuración básica
MONGO_URI = "mongodb://localhost:27017/"
DB_NAME = "citasMedicas"

def conectar_mongodb():
    """Establece conexión con MongoDB"""
    try:
        client = MongoClient(MONGO_URI, serverSelectionTimeoutMS=5000)
        client.admin.command('ping')  # Verifica conexión
        return client[DB_NAME]
    except Exception as e:
        print(f"Error de conexión a MongoDB: {str(e)}", file=sys.stderr)
        sys.exit(1)

def guardar_paciente(*args):
    """Guarda un paciente en MongoDB"""
    try:
        if len(args) != 7:
            raise ValueError("Se requieren 7 parámetros: nombre, apellido, cédula, dirección, correo, teléfono, sexo")
        
        db = conectar_mongodb()
        paciente = {
            "nombre": args[0],
            "apellido": args[1],
            "cedula": args[2],
            "direccion": args[3],
            "correo": args[4],
            "telefono": args[5],
            "sexo": args[6],
            "fecha_registro": datetime.now()
        }
        
        result = db.pacientes.insert_one(paciente)
        print(f"Paciente guardado ID: {result.inserted_id}")
        return True
    except Exception as e:
        print(f"Error guardando paciente: {str(e)}", file=sys.stderr)
        return False

def guardar_turno(*args):
    """Guarda un turno en MongoDB"""
    try:
        if len(args) != 14:
            raise ValueError("Se requieren 14 parámetros: [datos paciente(7)] + provincia, ciudad, día, mes, año, hora, minuto")
        
        db = conectar_mongodb()
        turno = {
            "paciente": {
                "nombre": args[0],
                "apellido": args[1],
                "cedula": args[2],
                "direccion": args[3],
                "correo": args[4],
                "telefono": args[5],
                "sexo": args[6]
            },
            "provincia": args[7],
            "ciudad": args[8],
            "fecha_hora": {
                "dia": int(args[9]),
                "mes": int(args[10]),
                "anio": int(args[11]),
                "hora": int(args[12]),
                "minuto": int(args[13])
            },
            "fecha_registro": datetime.now()
        }
        
        result = db.turnos.insert_one(turno)
        print(f"Turno guardado ID: {result.inserted_id}")
        return True
    except Exception as e:
        print(f"Error guardando turno: {str(e)}", file=sys.stderr)
        return False
    
def vaciar_base_datos():
    try:
        nombre_db = "citasMedicas"
        client = MongoClient("mongodb://localhost:27017/")
        db = client[nombre_db]
        
        colecciones = db.list_collection_names()
        for col in colecciones:
            db[col].drop()
            print(f"Colección '{col}' eliminada correctamente.")
        
        print(f"Base de datos '{nombre_db}' vaciada.")
    except Exception as e:
        print(f"Error al vaciar base de datos: {e}")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Uso: python servidor_intermedio.py <tipo> [parámetros...]", file=sys.stderr)
        print("Tipos: guardar_paciente | guardar_turno", file=sys.stderr)
        sys.exit(1)
    
    comando = sys.argv[1]
    parametros = sys.argv[2:]
    
    if comando == "guardar_paciente":
        if not guardar_paciente(*parametros):
            sys.exit(1)
    elif comando == "guardar_turno":
        if not guardar_turno(*parametros):
            sys.exit(1)
    elif comando == "vaciar_base_datos":
        vaciar_base_datos();
    else:
        print(f"Comando no reconocido: {comando}", file=sys.stderr)
        sys.exit(1)