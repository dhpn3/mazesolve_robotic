/****************************************************************************************************************
 *  BIBLIOTECAS
 ****************************************************************************************************************/

#include "Arduino.h"

#include "sensorUltrassonico.h"

/****************************************************************************************************************
 *  DEFINES E ENUMS
 ****************************************************************************************************************/

/**
 *  @brief  Pino do emissor referente ao sensor frontal
 */
#define PIN_TRIGGER_FRONTAL (13U)
/**
 *  @brief  Pino do receptor referente ao sensor frontal
 */
#define PIN_ECHO_FRONTAL (12U)
/**
 *  @brief  Pino do emissor referente ao sensor lateral direito
 */
#define PIN_TRIGGER_DIREITO (8U)
/**
 *  @brief  Pino do receptor referente ao sensor lateral direito
 */
#define PIN_ECHO_FRONTAL_DIREITO (7U)
/**
 *  @brief  Pino do emissor referente ao sensor lateral esquerdo
 */
#define PIN_TRIGGER_ESQUERDO (4U)
/**
 *  @brief  Pino do receptor referente ao sensor lateral esquerdo
 */
#define PIN_ECHO_FRONTAL_ESQUERDO (10U)

/**
 *  @brief  Distância máxima permitida para obstáculo frontal
 */
#define DISTANCIA_MAX_FRONTAL (20)
/**
 *  @brief  Distância máxima permitida para obstáculo frontal no processo de giro
 */
#define DISTANCIA_MAX_FRONTAL_PROC_GIRO (DISTANCIA_MAX_FRONTAL * 2)

/****************************************************************************************************************
 *  TYPEDEFS
 ****************************************************************************************************************/

/****************************************************************************************************************
 *  VARIÁVEIS
 ****************************************************************************************************************/

/**
 *  @brief  Armazena o estado do movimento do robô
 */
static estadosMovimentoRobo_t maquinaEstadosDirecaoRobo = EST0;

/****************************************************************************************************************
 *  PROTÓTIPOS DE FUNÇÃO
 ****************************************************************************************************************/

void configura_velocidade_robo(operacaoRobo_t * operacaoRobo, int leituraSensorFrontal);

/****************************************************************************************************************
 *  FUNÇÕES EXPORTADAS
 ****************************************************************************************************************/

void obtem_direcao_robo(operacaoRobo_t * operacaoRobo)
{
    /* Realiza a leitura dos sensores laterais */
    int leituraSensorFrontal    = distancia_sensor_frontal();
    int leituraSensorDireito    = distancia_sensor_direito();
    int leituraSensorEsquerdo   = distancia_sensor_esquerdo();

    switch(maquinaEstadosDirecaoRobo)
    {
        /*************************************************
         *  ESTADO 0 - ROBÔ PARADO
         *************************************************/
        case EST0:
        {
            /* Atualiza a operação dos motores */
            operacaoRobo->direcaoRobo = ROBO_PARADO;
            /* Atualiza a velocidade do robô */
            operacaoRobo->velocidadeRobo = RAPIDA_VELOCIDADE;
            /* Muda o estado de operação */
            maquinaEstadosDirecaoRobo = EST1;
        }break;
        /*************************************************
         *  ESTADO 1 - ROBÔ ANDANDO PARA FRENTE
         *************************************************/
        case EST1:
        {
            /* Atualiza a operação dos motores */
            operacaoRobo->direcaoRobo = ROBO_SEGUE_RETO;
            /* Atualiza a velocidade do robô */
            operacaoRobo->velocidadeRobo = RAPIDA_VELOCIDADE;
            /* Muda o estado de operação */
            maquinaEstadosDirecaoRobo = EST2;
        }
        break;
        /*************************************************
         *  ESTADO 2 - ROBÔ VERIFICA OBSTÁCULO À FRENTE
         *************************************************/
        case EST2:
        {
            if(leituraSensorFrontal < DISTANCIA_MAX_FRONTAL)
            {
                /* Atualiza a operação dos motores */
                operacaoRobo->direcaoRobo = ROBO_PARADO;
                /* Atualiza a velocidade do robô */
                operacaoRobo->velocidadeRobo = RAPIDA_VELOCIDADE;
                /* Muda o estado de operação */
                maquinaEstadosDirecaoRobo = EST3;
            }
            // else    // djhou - criar função de monitoramento 
            // {
            //     /* Continua andando reto */
            //     operacaoRobo.direcaoRobo = ROBO_SEGUE_RETO;

            //     caminho_alternativo();

            //     // seta flags de direção livre
            //     if(leituraSensorDireito < DISTANCIA_MAX_FRONTAL)
            //     {
            //         flagDireitaLivre = true;
            //         // contaAmostra++;
            //     }
            //     else if(leituraSensorEsquerdo < DISTANCIA_MAX_FRONTAL)
            //     {
            //         flagEsquerdaLivre = true;
            //         // contaAmostra++;
            //     }
            //     else
            //     {
            //         // direções direita e esquerda bloqueadas 
            //     }
            // }

            configura_velocidade_robo(operacaoRobo, leituraSensorFrontal);
        }break;
        /*************************************************
         *  ESTADO 3 - ROBÔ DECIDE QUAL LADO VIRAR
         *************************************************/
        case EST3:
        {
            /* aguarda tempo para manter o robô parado (talvez utilizar timer para acionar o estado anterior e pausar neste estado) */
            delay(1000);

            if(leituraSensorDireito < leituraSensorEsquerdo)
            {
                /* Como o sensor do lado direito está com a menor distância,
                * significa que há uma parede mais próxima deste lado, neste caso,
                * deve virar para o lado esquerdo
                */
                operacaoRobo->direcaoRobo = ROBO_VIRA_ESQUERDA;
            }
            else if(leituraSensorDireito > leituraSensorEsquerdo)
            {
                /* Como o sensor do lado esquerdo está com a menor distância,
                * significa que há uma parede mais próxima deste lado, neste caso,
                * deve virar para o lado direito
                */
                operacaoRobo->direcaoRobo = ROBO_VIRA_DIREITA;            
            }
            else
            {
                operacaoRobo->direcaoRobo = ROBO_MEIA_VOLTA;
            }
            /* Atualiza a velocidade do robô */
            operacaoRobo->velocidadeRobo = RAPIDA_VELOCIDADE;
            /* Muda o estado de operação */
            maquinaEstadosDirecaoRobo = EST4;
        }break;
        /*************************************************
         *  ESTADO 4 - PROCESSO DE GIRO
         *************************************************/
        case EST4:
        {
            if(leituraSensorFrontal < DISTANCIA_MAX_FRONTAL_PROC_GIRO)
            {
                /* Continua com a direção de giro e permanece neste estado */
                // if(flagEsquerdaLivre == true || flagDireitaLivre == true){
                //     // if(contaAmostra > debounceAmostras)
                //     // {
                //     // }
    
                // }
            }
            else
            {
                /* Inicializa a máquina de estados */
                maquinaEstadosDirecaoRobo = EST0;
                /* Atualiza a operação dos motores */
                operacaoRobo->direcaoRobo = ROBO_PARADO;
            }
        }break;

        default:
        {
            /* Não faz nada */
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
        operacaoRobo->velocidadeRobo = RAPIDA_VELOCIDADE;
    }
    else if((leituraSensorFrontal <  DISTANCIA_MAX_PARA_VELOCIDADE_MAXIMA_FRONTAL) && 
            (leituraSensorFrontal >= DISTANCIA_MAX_PARA_VELOCIDADE_MEDIA_FRONTAL))
    {
        operacaoRobo->velocidadeRobo = MEDIA_VELOCIDADE;
    }
    else if(leituraSensorFrontal < DISTANCIA_MAX_PARA_VELOCIDADE_MEDIA_FRONTAL)
    {
        operacaoRobo->velocidadeRobo = BAIXA_VELOCIDADE;
    }
}