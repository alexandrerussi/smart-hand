#include <Mindwave.h>
#include <SimpleTimer.h>
#include <Servo.h>

#define SERVO 6 // Porta Digital 6 PWM

Mindwave mindwave;

SimpleTimer simpleTimer;

Servo s;

int concentracaoNecessaria = 30;
int atencao = 0;
int intervaloPosicao = 10;

bool motorLiberado = true;

int posicaoAtual = 0;
int duracaoDelay = 1000;

int botao = 12;
bool leituraBotao = false;

void setup() {
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
  
  atualizarMotor();
}

void liberarMovimentacaoMotor()
{
	Serial.println("liberarMovimentacaoMotor");
	motorLiberado = true;
}

void atualizarMotor()
{
  atencao = mindwave.attention();
  
  if (atencao > concentracaoNecessaria)
  {
	// Avança uma posição

	alterarPosicaoServo(intervaloPosicao);
  }
  else
  {
	// Volta uma posição

	alterarPosicaoServo(intervaloPosicao * -1);
  }
}

void pressionarBotao()
{
  if (digitalRead(botao) == HIGH)
  {
    leituraBotao = true;
  }
  else if (digitalRead(botao) == LOW)
  {
    leituraBotao = false;
  }
}

void alterarPosicaoServo(intervaloPosicao)
{
  if ((motorLiberado == false) || (leituraBotao == true))
	return;
  
  Serial.println("alterarPosicaoServo");
  
  posicaoAtual = constrain(posicaoAtual + intervaloPosicao, 0, 180);

  s.write(posicaoAtual);

  motorLiberado = false;
  
  simpleTimer.setTimeout(duracaoDelay, liberarMovimentacaoMotor);
}

void loop()
{
  simpleTimer.run();

  mindwave.update(Serial, onMindwaveData);
  
  pressionarBotao();

  alterarPosicaoServo();

  atualizarMotor();
}
