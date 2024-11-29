/****************************************************************************************************************
 *  BIBLIOTECAS
 ****************************************************************************************************************/

#include <Arduino.h>
#include "motores.h"

#include "main.h"

/****************************************************************************************************************
 *  DEFINES E ENUMS
 ****************************************************************************************************************/

/**
 *  @brief  Velocidade de operação referente ao motor A
 */
#define VELOCIDADE_INICIAL_MOTOR_A  (255-135)
/**
 *  @brief  Velocidade de operação referente ao motor B
 */
#define VELOCIDADE_INICIAL_MOTOR_B  (245-135)

#define VELOCIDADE_BAIXA_MOTOR_A    (VELOCIDADE_INICIAL_MOTOR_A - 40)
#define VELOCIDADE_BAIXA_MOTOR_B    (VELOCIDADE_INICIAL_MOTOR_B - 40)

#define VELOCIDADE_MEDIA_MOTOR_A    (VELOCIDADE_INICIAL_MOTOR_A - 25)
#define VELOCIDADE_MEDIA_MOTOR_B    (VELOCIDADE_INICIAL_MOTOR_B - 25)

#define VELOCIDADE_ALTA_MOTOR_A     (VELOCIDADE_INICIAL_MOTOR_A)
#define VELOCIDADE_ALTA_MOTOR_B     (VELOCIDADE_INICIAL_MOTOR_B)

// definindo as portas onde estão conectados os motores 
#define IN1 2  //Pinos motor A (Direita)        
#define IN2 3  //Pinos motor A (Direita)                
#define IN4 5  //Pinos motor B (Esquerda)                    
#define IN3 6  //Pinos motor B (Esquerda)   

#define ENA 9  //Pino velocidade motor A (Enable A)              
#define ENB 11  //Pino velocidade motor B (Enable B)  

/****************************************************************************************************************
 *  TYPEDEFS
 ****************************************************************************************************************/

/****************************************************************************************************************
 *  VARIÁVEIS
 ****************************************************************************************************************/


/****************************************************************************************************************
 *  PROTÓTIPOS DE FUNÇÃO
 ****************************************************************************************************************/
void controla_velocidade_motor(velocidadesRobo_t velocidadeRobo);

/****************************************************************************************************************
 *  FUNÇÕES EXPORTADAS
 ****************************************************************************************************************/
void inicializa_motores(void)
{
    // Definindo os motores e outros componentes como saídas
    pinMode(IN1, OUTPUT); // Saída para motor A
    pinMode(IN2, OUTPUT); // Saída para motor A
    pinMode(IN3, OUTPUT); // Saída para motor B
    pinMode(IN4, OUTPUT); // Saída para motor B
    pinMode(ENA, OUTPUT); // Controle velocidade motor A
    pinMode(ENB, OUTPUT); // Controle velocidade motor B

    /* Velocidades dos motores */
    analogWrite(ENA, VELOCIDADE_INICIAL_MOTOR_A);
    analogWrite(ENB, VELOCIDADE_INICIAL_MOTOR_B);
}

void executarMovimentoMotor(operacaoRobo_t operacaoRobo)
{
    controla_velocidade_motor(operacaoRobo.velocidadeRobo);

    switch(operacaoRobo.direcaoRobo)
    {
        case ROBO_VIRA_ESQUERDA:
        {
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);
        }break;

        case ROBO_VIRA_DIREITA:
        {
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
        }break;
        
        case ROBO_SEGUE_RETO:
        {
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
        }break;
        case ROBO_MEIA_VOLTA:
        {
            /* Não utilizado */
        }break;
        
        case ROBO_PARADO:
        default:
        {
            /* Ré */
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);

            delay(150);

            digitalWrite(IN1, LOW);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, LOW);
        }break;
    }
}

/****************************************************************************************************************
 *  FUNÇÕES LOCAIS
 ****************************************************************************************************************/

/**
 *  @brief  Configura a velocidade do motor
 */
void controla_velocidade_motor(velocidadesRobo_t velocidadeRobo)
{
    switch (velocidadeRobo)
    {
        case BAIXA_VELOCIDADE:
        {
            analogWrite(ENA, VELOCIDADE_BAIXA_MOTOR_A);
            analogWrite(ENB, VELOCIDADE_BAIXA_MOTOR_B);
        }break;

        case MEDIA_VELOCIDADE:
        {
            analogWrite(ENA, VELOCIDADE_MEDIA_MOTOR_A);
            analogWrite(ENB, VELOCIDADE_MEDIA_MOTOR_B);
        }break;

        case RAPIDA_VELOCIDADE:
        default:
        {
            analogWrite(ENA, VELOCIDADE_ALTA_MOTOR_A);
            analogWrite(ENB, VELOCIDADE_ALTA_MOTOR_B);
        }break;
    }
}