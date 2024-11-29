/****************************************************************************************************************
 * 
 * Referências:
 * 
 * https://github.com/ErickSimoes/Ultrasonic/blob/master/README_pt-br.md
 * 
 * MIT License
 * 
 * Copyright (c) 2017 Erick Simões
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 ****************************************************************************************************************/

/****************************************************************************************************************
 *  BIBLIOTECAS
 ****************************************************************************************************************/

#include "main.h"

#include "sensorUltrassonico.h"
#include "motores.h"

/****************************************************************************************************************
 *  DEFINES E ENUMS
 ****************************************************************************************************************/

// #define TESTE_SENSORES

/****************************************************************************************************************
 *  VARIÁVEIS GLOBAIS
 ****************************************************************************************************************/

/**
 *  @brief  Estrutura responsável pela operação do robô
 */
static operacaoRobo_t operacaoRobo;

/****************************************************************************************************************
 *  FUNÇÕES LOCAIS
 ****************************************************************************************************************/

/**
 *  @brief  Inicialização
 */
void setup()
{
    /* Inicializa comunicação serial */
    Serial.begin(9600);
    /* Limpando a estrutura */
    memset(&operacaoRobo, 0, sizeof(operacaoRobo));

    inicializa_motores();
                         
    delay(1000);            // Aguarda 1 segundo antes de iniciar  
}

/**
 *  @brief  Loop principal
 */
void loop()
{
#ifdef TESTE_SENSORES
    /* Leitura sensor frontal */
    Serial.print("Frontal: ");
    Serial.print(distancia_sensor_frontal());
    Serial.println(" cm");

    /* Leitura sensor lateral direito */
    Serial.print("Direito: ");
    Serial.print(distancia_sensor_direito());
    Serial.println(" cm");

    /* Leitura sensor lateral esquerdo */
    Serial.print("Esquerdo: ");
    Serial.print(distancia_sensor_esquerdo());
    Serial.println(" cm");

    delay(1000);
#else
    obtem_direcao_robo(&operacaoRobo);

    executarMovimentoMotor(operacaoRobo);

    delay(1);
#endif
}
