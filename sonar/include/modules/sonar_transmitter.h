#ifndef SONAR_TRANSMITTER_H
#define SONAR_TRANSMITTER_H

#include <Arduino.h>
#include <HardwareSerial.h>
#include "config.h"

class SonarTransmitter {
public:
    // Constructor
    SonarTransmitter();
    
    // Métodos principales
    bool begin();
    void update();
    void addSonarMeasurement(double depth, double offset, double range, uint32_t totalLog, uint32_t tripLog, float temperature);

    // Configuración
    void setTransmissionInterval(unsigned long intervalMs);
    
    // Estado
    bool isConnected() const;
    unsigned long getLastTransmissionTime() const;
    int getMeasurementCount() const;

private:
    // Comunicación serial
    HardwareSerial* dataloggerSerial;
    
    // Control de tiempo
    unsigned long transmissionInterval_;      // Intervalo de transmisión (500ms por defecto)
    unsigned long lastTransmissionTime_;
    
    // Datos para promedio
    struct SonarData {
        double depth;
        double offset;
        double range;
        uint32_t totalLog;
        uint32_t tripLog;
        float temperature;
        bool valid;
    };
    
    static const int MAX_MEASUREMENTS = 100;  // Máximo de mediciones a promediar
    SonarData measurements_[MAX_MEASUREMENTS];
    int measurementCount_;
    int writeIndex_;              // Índice donde escribir el próximo dato
    int validDataCount_;          // Cuántos datos válidos hay en el buffer
    bool bufferFull_;            // Si el buffer ha dado una vuelta completa
       
    // Métodos privados
    void addToCircularBuffer(const SonarData& data);
    void calculateAndTransmitAverage();
    SonarData calculateAverage();
    void transmitData(const SonarData& data);
    String formatDataPacket(const SonarData& data);
    void resetMeasurements();
    
    // Validación
    bool isValidMeasurement(double depth, double offset, double range, float temperature); 
};

#endif // SONAR_TRANSMITTER_H