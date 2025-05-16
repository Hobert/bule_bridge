#include "key_bsp.h"

uint8_t key_adc_flag = 0;

uint8_t key_val = 0;  // ��ǰ����״̬
uint8_t key_old = 0;  // ǰһ����״̬
uint8_t key_down = 0; // ��¼���µİ���
uint8_t key_up = 0;   // ��¼�ͷŵİ���

uint8_t index = 0;

uint8_t fulse_change_flag = 0;

uint32_t record_time;

uint32_t b2_last_time = 0;

uint8_t duty_change_lock = 0;

/**
 * @brief ��ȡ����״̬
 *
 * �ú�����ȡ������ GPIO �����ϵİ���״̬����������Ӧ�İ�����š�?
 *
 * @return ���ذ�����š�?0 ��ʾû�а������£�1-4 ��ʾ��Ӧ�İ��������¡�
 */
uint8_t key_read(void)
{
  // ���ڴ洢����״̬����ʱ����
  uint8_t temp = 0;

  // ���? GPIOB ���� 0 ��״̬
  if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0) == GPIO_PIN_RESET)
    temp = 1; // �������״�?Ϊ RESET���򰴼� 1 ������

  // ���? GPIOB ���� 1 ��״̬
  if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET)
    temp = 2; // �������״�?Ϊ RESET���򰴼� 2 ������

  // ���? GPIOB ���� 2 ��״̬
  if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2) == GPIO_PIN_RESET)
    temp = 3; // �������״�?Ϊ RESET���򰴼� 3 ������

  // ���? GPIOA ���� 0 ��״̬
  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)
    temp = 4; // �������״�?Ϊ RESET���򰴼� 4 ������

  // ���ؼ�⵽�İ������
  return temp;
}

/**
 * @brief ������������
 *
 * �ú�������ɨ�谴����״̬�������°������º��ͷŵı�־
 */
void key_proc(void)
{
  // ��ȡ��ǰ����״̬
  key_val = key_read();
  // ���㰴�µİ�������ǰ����״̬��ǰһ״̬���?���뵱ǰ״̬���룩
  key_down = key_val & (key_old ^ key_val); // Ϊ1��λΪ����״̬
  // �����ͷŵİ�������ǰδ����״̬��ǰһ״̬���?����ǰһ״̬���룩
  key_up = ~key_val & (key_old ^ key_val); // Ϊ1��λΪ�ͷ�״̬
  // ����ǰһ����״̬
  key_old = key_val;

  if (key_up == 4)
  {
    if (Lcd_display_mode == 0)
    {
      if (HAL_GetTick() - record_time > 2000)
      {
        duty_change_lock = 1;
      }
    }
  }

  switch (key_down)
  {
  case 1:
    Lcd_display_mode = (Lcd_display_mode + 1) % 3;
    if (Lcd_display_mode == 2)
    {
      set_value[0] = set_dis_value[0];
      set_value[1] = set_dis_value[1];
    }
    break;

    case 2: // 定义为“选择”按键
    // 切换选择低频或高频模式
    // 按键按下后，5秒内不可再次触发切换功能。
    if (Lcd_display_mode == 0)
    {

      if (HAL_GetTick() - b2_last_time > 5000 || n == 0)
      {
        fulse_change_flag = 1;
        b2_last_time = HAL_GetTick();
        n++;
      }
    }

    if (Lcd_display_mode == 1)
    {
      index ^= 1;
    }

    break;
  case 3: // 定义为“加”按键
    if (Lcd_display_mode == 1)
      set_dis_value[index] += 1;
    else if (Lcd_display_mode == 1 && set_dis_value[index] == 11)
      set_dis_value[index] = 0;

    break;
  case 4: // 定义为“减”按键
    if (Lcd_display_mode == 0)
    {
      record_time = HAL_GetTick();
    }

    break;
  }
  if (key_up == 4)
  {
    if (uwTick - record_time > 2000) // 长按逻辑
    {
      duty_change_lock = 1; // 锁定功能
    }
    else if (Lcd_display_mode == 1) // 短按逻辑
    {
      set_dis_value[index] -= 1;
      if (set_dis_value[index] == 0)
        set_dis_value[index] = 1;
    }
    else // 短按解锁逻辑
    {
      duty_change_lock = 0; // 解锁
    }
  }
}

// ����״̬����������

typedef struct
{
  GPIO_TypeDef *gpiox; // ָ�� GPIO �˿ڵ�ָ�룬���� GPIOA��GPIOB �ȡ�
  uint16_t pin;        // ָ�� GPIO ���ţ����� GPIO_PIN_0��GPIO_PIN_1 �ȡ�
  uint16_t ticks;      // ���ڼ�ʱ�ı�����ͨ������ȥ����������
  uint8_t level;       // ��ǰ�����ĵ�ƽ״̬���ߵ�ƽ��͵�ƽ��?
  uint8_t id;          // ������Ψһ��ʶ���������������ֲ�ͬ�İ�����
  uint8_t state;       // �����ĵ�ǰ״̬���������ڱ�ʾ�����Ƿ񱻰��»��ͷš�
  uint8_t debouce_cnt; // ����ȥ���������������ڷ�ֹ��������������󴥷���?
  uint8_t repeat;      // �����ظ����µĴ�����
} button;

button btns[4]; // ��������

// ������ʼ������
void key_init(void)
{
  // ��ʼ����һ������
  btns[0].gpiox = GPIOB;    // ָ��GPIO�˿�
  btns[0].pin = GPIO_PIN_0; // ָ������
  btns[0].level = 1;        // ���ó�ʼ��ƽ
  btns[0].id = 0;           // ���ð���ID

  // ��ʼ���ڶ�������
  btns[1].gpiox = GPIOB;
  btns[1].pin = GPIO_PIN_1;
  btns[1].level = 1;
  btns[1].id = 1;

  // ��ʼ������������
  btns[2].gpiox = GPIOB;
  btns[2].pin = GPIO_PIN_2;
  btns[2].level = 1;
  btns[2].id = 2;

  // ��ʼ�����ĸ�����
  btns[3].gpiox = GPIOA;
  btns[3].pin = GPIO_PIN_0;
  btns[3].level = 1;
  btns[3].id = 3;
}

// ��������������
void key_task(button *btn)
{
  // ��ȡ������ǰ��ƽ
  uint8_t gpio_level = HAL_GPIO_ReadPin(btn->gpiox, btn->pin);

  // �������״�?���� 0���������ʱ��?
  if (btn->state > 0)
    btn->ticks++;

  // �����ǰ��ƽ�밴����¼�ĵ�ƽ���?������ȥ��������
  if (btn->level != gpio_level)
  {
    // �����ﵽ 3 �Σ�ȷ�ϵ�ƽ�仯
    if (++(btn->debouce_cnt) >= 3)
    {
      btn->level = gpio_level; // ���µ�ƽ
      btn->debouce_cnt = 0;    // ����ȥ����������
    }
  }
  else
  {
    btn->debouce_cnt = 0; // ��ƽû�б仯������ȥ����������
  }

  // ����״̬��
  switch (btn->state)
  {
  case 0:                // ��ʼ״̬
    if (btn->level == 0) // �ȴ���������
    {
      btn->ticks = 0;  // ���ü�ʱ��
      btn->repeat = 1; // ��ʼ�����ظ�����
      btn->state = 1;  // ���밴������״̬
    }
    break;
  case 1:                // ��������״̬
    if (btn->level != 0) // �ȴ������ɿ�
    {
      if (btn->ticks >= 30) // ��������
      {

        ucled[btn->id] ^= 1; // ִ�г�����Ĳ���?
        btn->state = 0;      // ���س�ʼ״̬
      }
      else
      {
        btn->ticks = 0; // ���ü�ʱ��
        btn->state = 2; // ���밴���ͷ�״̬
      }
    }
    else if (btn->ticks >= 30) // ��������
    {

      btn->repeat = 0; // ��ֹ�ͷŵ�ʱ���ٴδ��������¼�
    }
    break;
  case 2:                 // �����ͷ�״̬
    if (btn->ticks >= 15) // ��ʱ���ﵽ��ֵ
    {
      btn->state = 0; // ���س�ʼ״̬
      if (btn->repeat == 1)
      {
        ucled[btn->id + 4] ^= 1; // ������Ӧ��LED
      }
      else if (btn->repeat == 2)
      {
        ucled[btn->id] ^= 1; // ������Ӧ��LED
      }
    }
    else
    {
      if (btn->level == 0) // �����ٴΰ���
      {
        btn->repeat++;  // �����ظ�����
        btn->ticks = 0; // ���ü�ʱ��
        btn->state = 1; // ���ذ�������״̬
      }
    }
    break;
  }
}

// ����״̬��������
void key_state(void)
{
  for (uint8_t i = 0; i < 4; i++) // �������а���
  {
    key_task(&btns[i]); // ����ÿ��������״̬
  }
}
