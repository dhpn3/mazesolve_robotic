#include "Arduino.h"
#include "sensorUltrassonico.h"
#include <motores.h>

#define PIN_TRIGGER_FRONTAL (13U)
#define PIN_ECHO_FRONTAL (12U)
#define PIN_TRIGGER_DIREITO (8U)
#define PIN_ECHO_FRONTAL_DIREITO (7U)
#define PIN_TRIGGER_ESQUERDO (4U)
#define PIN_ECHO_FRONTAL_ESQUERDO (10U)

#define DISTANCIA_MAX_FRONTAL (15)
#define DISTANCIA_MAX_FRONTAL_PROC_GIRO (DISTANCIA_MAX_FRONTAL * 2)

static estadosMovimentoRobo_t maquinaEstadosDirecaoRobo = EST0;

void configura_velocidade_robo(operacaoRobo_t * operacaoRobo, int leituraSensorFrontal);


void obtem_direcao_robo(operacaoRobo_t * operacaoRobo)
{
    /* Realiza a leitura dos sensores laterais */
    int leituraSensorFrontal    = distancia_sensor_frontal();
    int leituraSensorDireito    = distancia_sensor_direito();
    int leituraSensorEsquerdo   = distancia_sensor_esquerdo();

    static uint8_t old_state = 0;

    switch(maquinaEstadosDirecaoRobo)
    {
        case EST0:
        {
            /* Inicializa a operação dos motores */
            operacaoRobo->direcaoRobo = ROBO_PARADO;
            /* Atualiza a velocidade do robô */
            operacaoRobo->velocidadeRobo = MEDIA_VELOCIDADE;
            /* Muda o estado de operação */
            maquinaEstadosDirecaoRobo = EST1;
        }break;

        case EST1:
        {
            /* Atualiza a operação dos motores */
            operacaoRobo->direcaoRobo = ROBO_SEGUE_RETO;
            /* Atualiza a velocidade do robô */
            operacaoRobo->velocidadeRobo = MEDIA_VELOCIDADE;
            /* Muda o estado de operação */
            maquinaEstadosDirecaoRobo = EST2;
        }
        break;

        case EST2:
        {
            // O robô segue em frente até detectar um obstáculo
            if(leituraSensorFrontal < DISTANCIA_MAX_FRONTAL)
            {
                operacaoRobo->direcaoRobo = ROBO_PARADO;
                maquinaEstadosDirecaoRobo = EST3; // Estado para avaliação dos sensores laterais
            }
            else
            {
                // Se o sensor lateral esquerdo estiver livre, o robô deve virar à esquerda
                if (leituraSensorEsquerdo > 30 && leituraSensorEsquerdo < 70)   // por ex: 50
                {
                    // operacaoRobo->direcaoRobo = ROBO_VIRA_ESQUERDA;
                    old_state = ROBO_VIRA_ESQUERDA;
                    operacaoRobo->direcaoRobo = ROBO_PARADO;
                    maquinaEstadosDirecaoRobo = EST3;
                }
                else if(leituraSensorEsquerdo > 70){
                    // esquerda infinita
                }
                // Caso contrário, se o direito estiver livre, o robô vira para a direita
                else if (leituraSensorDireito > 30 && leituraSensorDireito < 70)
                {
                    // operacaoRobo->direcaoRobo = ROBO_VIRA_DIREITA;
                    old_state = ROBO_VIRA_DIREITA;
                    operacaoRobo->direcaoRobo = ROBO_PARADO;
                    maquinaEstadosDirecaoRobo = EST3;
                }
                // Caso contrário, o robô segue reto
                else
                {
                    old_state = ROBO_SEGUE_RETO;
                    operacaoRobo->direcaoRobo = ROBO_SEGUE_RETO;
                    operacaoRobo->velocidadeRobo = BAIXA_VELOCIDADE;

                }
            }
            // configura_velocidade_robo(operacaoRobo, leituraSensorFrontal);
        }break;

        case EST3:
        {
            // Aguarda tempo para processar a situação (gerenciamento de espera)
            delay(500);

            // Após o robô parar, verifica novamente qual direção escolher
            if(leituraSensorEsquerdo > leituraSensorDireito || old_state == ROBO_VIRA_ESQUERDA)
            {
                operacaoRobo->direcaoRobo = ROBO_VIRA_ESQUERDA;
                old_state = 0;  // reset
            }
            else if(leituraSensorDireito > leituraSensorEsquerdo || old_state == ROBO_VIRA_DIREITA)
            {
                operacaoRobo->direcaoRobo = ROBO_VIRA_DIREITA;
                old_state = 0;  // reset
            }
            else
            {
                // Robô encurralado. Dando meia volta...
                operacaoRobo->direcaoRobo = ROBO_MEIA_VOLTA;

                // fazer oq apos isso?
                // atualmente ta indo pro EST4
            }
            operacaoRobo->velocidadeRobo = MEDIA_VELOCIDADE;
            maquinaEstadosDirecaoRobo = EST4;
        }break;

        case EST4:
        {
            // Continua processando até encontrar um caminho livre
            if(leituraSensorFrontal >= DISTANCIA_MAX_FRONTAL_PROC_GIRO)
            {
                maquinaEstadosDirecaoRobo = EST0;  // Retorna para o estado inicial para seguir em frente novamente
                operacaoRobo->direcaoRobo = ROBO_PARADO;
            }
            else{
                // ainda nao desbloqueou o frontal, continua dando meia volta
                // if(leituraSensorEsquerdo > 30)
                // operacaoRobo->direcaoRobo = 
            }
        }break;

        default:
        {
            // Não faz nada
        }
        break;
    }
}


/****************************************************************************************************************
 *  FUNÇÕES LOCAIS
 ****************************************************************************************************************/

int distancia_sensor_frontal(void)
{
    Ultrasonic sensorFrontal(PIN_TRIGGER_FRONTAL, PIN_ECHO_FRONTAL);
    return sensorFrontal.read(CM);
}

int distancia_sensor_direito(void)
{
    Ultrasonic sensorDireito(PIN_TRIGGER_DIREITO, PIN_ECHO_FRONTAL_DIREITO);
    return sensorDireito.read(CM);
}

int distancia_sensor_esquerdo(void)
{
    Ultrasonic sensorEsquerdo(PIN_TRIGGER_ESQUERDO, PIN_ECHO_FRONTAL_ESQUERDO);
    return sensorEsquerdo.read(CM);
}

/**
 *  @brief  Atualiza a velocidade do robô a partir da distância frontal
 */
void configura_velocidade_robo(operacaoRobo_t * operacaoRobo, int leituraSensorFrontal)
{
#define DISTANCIA_MAX_PARA_VELOCIDADE_MAXIMA_FRONTAL    (60)
#define DISTANCIA_MAX_PARA_VELOCIDADE_MEDIA_FRONTAL     (45)

    if(leituraSensorFrontal >= DISTANCIA_MAX_PARA_VELOCIDADE_MAXIMA_FRONTAL)
    {
        operacaoRobo->velocidadeRobo = MEDIA_VELOCIDADE;
    }
    else if((leituraSensorFrontal <  DISTANCIA_MAX_PARA_VELOCIDADE_MAXIMA_FRONTAL) && 
            (leituraSensorFrontal >= DISTANCIA_MAX_PARA_VELOCIDADE_MEDIA_FRONTAL))
    {
        operacaoRobo->velocidadeRobo = MEDIA_VELOCIDADE;
    }
    else if(leituraSensorFrontal < DISTANCIA_MAX_PARA_VELOCIDADE_MEDIA_FRONTAL)
    {
        operacaoRobo->velocidadeRobo = MEDIA_VELOCIDADE;
    }
}