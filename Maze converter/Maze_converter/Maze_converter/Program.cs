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
			for (int i = 0; i < 48; i++) {
				input [i] = new char[84];
			}
			string filename;
			filename = Console.ReadLine ();
			using (StreamReader sr = new StreamReader (filename)) {
				for (int i = 0; i < 48; i++) {
					for (int j = 0; j < 84; j++) {
						input [i] [j] = Convert.ToChar (sr.Read ());
					}
				}
			}
			using (StreamWriter sw = new StreamWriter (filename + ".o")) {
				sw.Write ("{\n");
				for (int i = 0; i < 48; i++) {
					sw.Write ("{");
					for (int j = 0; j < 84; j++) {
						sw.Write (input [i] [j]);
						if (j < 83)
							sw.Write (",");
					}
					sw.Write ("}");
					if (i < 47)
						sw.Write (",\n");
				}
				sw.Write ("\n};");
			}
			;
		}
	}
}
