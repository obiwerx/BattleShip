##Created by StudioOB on 4/5/13.                                        
##Copyright (c) 2013 Tim O'Brien. All rights reserved.                 

 

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or 
(at your option) any later version.                                  

This program is distributed in the hope that it will be useful,      
but WITHOUT ANY WARRANTY; without even the implied warranty of       
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        
GNU General Public License for more details.                         

You should have received a copy of the GNU General Public License    
along with this program.  If not, see <http://www.gnu.org/licenses/>.

###KNOWN BUGS IN THIS VERSION                  
1) character validation is not working correctly - it is 
possible to enter a character when the program expects
a int value and crash the game.  Still researching the
best way to handle this.  (isdigit and isalpha)       
                                                         
2) The #Defines's for the color codes only work in a UNIX
Environment - they don't render correctly in Windows. 
                                                         
3) I have has problems with player records skipping over 
record 0 - I haven't been able to find a solution to  
this problem when creating a new player with nothing  
in the player.dat file.  I'll have to spend more debug
time tracking down a solution.                        
                                                         
4) Major trouble in the honor roll - it just appending   
any records from players that complete the game rather
than only publishing the top 10 scores.. etc..  I was 
planning something here that I just didn't have the   
time to execute properly.  At least it functions some-
what.  I'd like to have it format just the handle and 
that looked to see if the score belonged in the list. 
