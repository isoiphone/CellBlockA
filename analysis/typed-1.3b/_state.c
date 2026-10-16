/* Original 1.3b CELL.EXE address: 0000b8a0 */

char * _state(int param_1)

{
  char *pcVar1;
  
  switch(_player[param_1].state) {
  case 0:
    pcVar1 = "stand";
    break;
  case 1:
    pcVar1 = "crouch";
    break;
  case 2:
    pcVar1 = "run";
    break;
  case 3:
    pcVar1 = "roll";
    break;
  case 4:
    pcVar1 = "climb";
    break;
  case 5:
    pcVar1 = "jump";
    break;
  case 6:
    pcVar1 = "attack";
    break;
  case 7:
    pcVar1 = "die1";
    break;
  case 8:
    pcVar1 = "die2";
    break;
  case 9:
    pcVar1 = "die3";
    break;
  default:
    pcVar1 = "error";
  }
  return pcVar1;
}

