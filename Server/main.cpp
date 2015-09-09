/***************************************************************************
 *   Copyright (C) 2006 by Bai Aijun   *
 *   baj@mail.ustc.edu.cn,   baj_forever@126.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/**
	@author Bai Aijun <baj@mail.ustc.edu.cn,   baj_forever@126.com>
 */


#include "TCPServer.h"
#include "board.h"
extern "C"{
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#if 0
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif
}

void version()
{
	printf(">>>> Reversi Server version %s, by Bai Aijun\n", "0.1");
	puts("my email: <baj@mail.ustc.edu.cn, baj_forever@126.com>");
}

#if 0
int daemon_init()
{
	pid_t pid;
	if((pid = fork()) < 0)
		return -1;
	else if(pid != 0)
		exit(0);
	setsid();
	chdir("/tmp");
	umask(0);
	return 0;
}
#endif


int main()
{
#if 0
	if(daemon_init() != 0)
		puts("daemon_init failed");
	else
		puts("daemon inited");
#endif
	version();
	(void) new TCPServer(new Board);
}
