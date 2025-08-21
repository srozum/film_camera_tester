#include "Timer2.h"

Timer2::Timer2()
{
  overflow_count = 0;
}

unsigned long Timer2::Count::total_count()
{
  return (overflow_count << 8) + count;
}

void Timer2::start()
{
  // backup variables
  _tccr2a_save = TCCR2A;
  _tccr2b_save = TCCR2B;

  // Reset Timer 2
  TCCR2A = 0;
  TCCR2B = 0;

  reset();

  // Enable Timer2 overflow interrupt
  TIMSK2 |= _BV(TOIE2);

  // Setup and start timer
  TCCR2B = bit(CS21); // Prescaler 8
}

void Timer2::stop()
{
  TIMSK2 &= ~(_BV(TOIE2));
  TCCR2A = _tccr2a_save;
  TCCR2B = _tccr2b_save;
}

void Timer2::reset()
{
  overflow_count = 0;
  TCNT2 = 0;
  TIFR2 |= bit(TOV2);
}

// get total count for Timer2
void Timer2::get_count(Timer2::Count *count)
{
  uint8_t SREG_old = SREG;
  noInterrupts();

  count->count = TCNT2;

  if (TIFR2 & bit(TOV2))
  {
    count->count = TCNT2;
    overflow_count++;
    TIFR2 |= bit(TOV2);
  }

  SREG = SREG_old;

  count->overflow_count = overflow_count;
}

void Timer2::increment_overflow_count()
{
  overflow_count++;
}
