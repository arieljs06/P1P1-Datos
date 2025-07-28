#!/usr/bin/env python3
from pymongo import MongoClient
import sys

def eliminar_turnos_por_cedula(cedula):
    """Elimina todos los turnos asociados a una cédula en MongoDB"""
    try:
        # 1. Conexión a MongoDB
        client = MongoClient('mongodb://localhost:27017/', serverSelectionTimeoutMS=5000)
        db = client['citasMedicas']
        
        # 2. Buscar y eliminar turnos
        result = db.turnos.delete_many({"paciente.cedula": cedula})
        
        if result.deleted_count > 0:
            print(f"Se eliminaron {result.deleted_count} turnos para la cédula {cedula}")
            return True
        else:
            print(f"No se encontraron turnos para la cédula {cedula}")
            return False
            
    except Exception as e:
        print(f"Error al eliminar turnos: {str(e)}", file=sys.stderr)
        return False

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Uso: python eliminar_turno.py <cedula>", file=sys.stderr)
        sys.exit(1)
    
    cedula = sys.argv[1]
    if not eliminar_turnos_por_cedula(cedula):
        sys.exit(1)