#include "main.h"

#include "sensorUltrassonico.h"
#include "motores.h"

/**
 *  @brief  Estrutura responsável pela operação do robô
 */
static operacaoRobo_t operacaoRobo;

void setup()
{
    /* Inicializa comunicação serial */
    Serial.begin(9600);
    /* Limpando a estrutura */
    memset(&operacaoRobo, 0, sizeof(operacaoRobo));

    inicializa_motores();
                         
    delay(1000);            // Aguarda 1 segundo antes de iniciar  
}

void loop()
{
    obtem_direcao_robo(&operacaoRobo);

    executarMovimentoMotor(operacaoRobo);

    delay(1);
}