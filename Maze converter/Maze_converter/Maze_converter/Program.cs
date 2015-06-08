using System;
using System.IO;
using System.Collections.Generic;

namespace Maze_converter
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			char[][] input = new char[48][];
			for (int y = 0; y < 48; y++) {
				input [y] = new char[84];
			}
			string filename;
			filename = Console.ReadLine ();
			using (StreamReader sr = new StreamReader (filename)) {
				for (int y = 0; y < 48; y++) {
					for (int x = 0; x < 84; x++) {
						input [y] [x] = Convert.ToChar (sr.Read ());
					}
				}
			}
			using (StreamWriter sw = new StreamWriter (filename + ".o")) {
				for (int y = 1; y < 47; y++) {
					sw.Write ("");
					for (int x = 1; x < 83; x++) {
						if (input [y] [x] == '1') {
							Console.Write (input [y] [x]);
							sw.Write (" or ( x == " + x + " and y == " + y + ")");
						}
					}
				}
			}
		}
	}
}
