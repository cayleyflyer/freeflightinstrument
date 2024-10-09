char direction1[] = {'N','E','S','S','W','N'};
char direction2[][3] = {"NE","SE","SW","NW"};

char *degrees2direction1(int degrees)
{
   static char ret[4];
   
   degrees = degrees % 360;
   byte idx = (degrees*10 + 112)/225;

   byte c1 = idx/3;
   byte c2 = idx/4;
   
   if( idx == 11 )
   {
      c1++;
   }

   switch( idx % 4 )
   {
      case 0:
         sprintf(ret, "%c", direction1[c1]);
         break;
      case 1:
      case 3:
         sprintf(ret, "%c%s", direction1[c1], direction2[c2]);
         break;
      case 2:
         sprintf(ret, "%s", direction2[c2]);
         break;
   }
 
   return ret;
}