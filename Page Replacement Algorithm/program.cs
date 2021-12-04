using System;
using System.Collections.Generic;
using System.Collections;
using System.Text;
using System.Linq;
using System.IO;
namespace PA4_571
{
    public class Program
    {
        static void Main(string[] args)
        {
            System.IO.StreamReader file = new System.IO.StreamReader("data1.txt");
            System.IO.StreamReader file2 = new System.IO.StreamReader("data2.txt");
            PageEntry[] mainMem = new PageEntry[32];
            List<PageTable> tables = new List<PageTable>();
            int countF = 0;
            int countR = 0;
            int countD = 0;
            Console.Write("Enter 1 to read data1. Enter 2 to read data2.\n");
            int choice = Convert.ToInt32(Console.ReadLine());
            string line = "";
            if (choice == 1)
            {
                line = file.ReadLine();
            }
            else if (choice == 2)
            {
                line = file2.ReadLine();
            }
            else
            {
                Console.Write("Invalid choice.");
                return;
            }
            int processNum, vpn, dirty;
            while(line!=null)
            {
                string[] data = line.Split(' ');
                processNum = Convert.ToInt32(data[0]);
                vpn = Convert.ToInt32(data[1]) >> 9;
                char wr = Convert.ToChar(data[2]);
                if( wr == 'W')
                {
                    dirty = 1;
                }
                else
                {
                    dirty = 0;
                }
                PageEntry page = new PageEntry();
                page.setVPN(vpn);
















                if (choice == 1)
                {
                    line = file.ReadLine();
                }
                else if (choice == 2)
                {
                    line = file2.ReadLine();
                }
            }
            file.Close();
        }
    }
}
