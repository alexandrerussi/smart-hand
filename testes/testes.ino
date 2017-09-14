#include <Mindwave.h>
#include <SimpleTimer.h>
#include <Servo.h>

#define SERVO 6 // Porta Digital 6 PWM

Mindwave mindwave;

SimpleTimer simpleTimer;

Servo s;

int atencao = 0;
int concentracaoNecessaria = 30;
bool atencaoMotor = true;

int botao = 12;
int estadoBotao;

int posicaoMotor;
int intervaloPosicao;

void setup()
{
	pinMode(botao, INPUT);

	s.attach(SERVO);
	Serial.begin(9600);
  	s.write(0); // Inicia motor posição zero
  	Serial.begin(MINDWAVE_BAUDRATE);
}

void onMindwaveData()
{
  Serial.print("\tquality: ");
  Serial.print(mindwave.quality());
  Serial.print("\tattention: ");
  Serial.print(mindwave.attention());
  Serial.print("\tmeditation: ");
  Serial.print(mindwave.meditation());
  Serial.print("\tlast update: ");
  Serial.print(mindwave.lastUpdate());
  Serial.print("ms ago");
  Serial.println();
}

void condicaoAtencao()
{
	atencao = mindwave.attention();

	if (attention > concentracaoNecessaria)
	{
		atencaoMotor = true;
	}
	else
	{
		atencaoMotor = false;
	}
}

void apertaBotao()
{
	estadoBotao = digitalRead(botao);
}

void liberarMotor()
{
	if (estadoBotao == HIGH)
	{
		return;
	}
	else if (estadoBotao == LOW)
	{
		if (atencaoMotor == true)
		{
			posicaoMotor = constrain(posicaoMotor + intervaloPosicao, 0, 180);
			s.write(posicaoMotor);
		}
		else if (atencaoMotor == false)
		{
			posicaoMotor = constrain(posicaoMotor - intervaloPosicao, 0, 180);
			s.write(posicaoMotor);
		}
	}
}

void loop()
{
	mindwave.update(Serial, onMindwaveData);

	condicaoAtencao();
	apertaBotao();
	liberarMotor();
}