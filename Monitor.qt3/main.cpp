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


#include "monitor.h"

void version()
{
	printf(">>>> Reversi Monitor version %s, by Bai Aijun\n", "0.1");
	puts("my email: <baj@mail.ustc.edu.cn, baj_forever@126.com>");
}

void usage(char *name)
{
	fprintf(stderr, "usage: %s [IpAddr]\n", name);
}

int main( int argc, char *argv[] )
{
	version();	
	if(argc > 2){
		usage(argv[0]);
		exit(1);
	}	
	char *host = "127.0.0.1";
	if(argc == 2 ){
		if( strlen(argv[1]) > 255){
			fprintf(stderr, "too long IpAddr %s\n", argv[1]);
			exit(1);
		}
		else 
			host = argv[1];
	}
	
	QApplication app( argc, argv );
	Monitor *monitor = new Monitor(host);	
	app.setMainWidget( monitor );	
	monitor->show();
	return app.exec();	
}
