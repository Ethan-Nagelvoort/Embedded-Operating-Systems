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
            MainMem mm = new MainMem();
            PageEntry[] mainMem = new PageEntry[32];
            List<PageTable> tables = new List<PageTable>();
            List<string> lines = new List<string>();
            int countF = 0;
            int countR = 0;
            int countD = 0;
            Random r = new Random();
            Console.Write("Enter 1 to read data1. Enter 2 to read data2.\n");
            int choice = Convert.ToInt32(Console.ReadLine());
            Console.Write("Enter 1 to use Rand.\nEnter 2 to use FIFO.\nEnter 3 to use LRU.\nEnter 4 to use PER.\n");
            int repChoice = Convert.ToInt32(Console.ReadLine());
            //parse file into list of strings
            string line = "";
            if (choice == 1)
            {
                line = file.ReadLine();
                while(line!= null)
                {
                    lines.Add(line);
                    line = file.ReadLine();
                }
            }
            else if (choice == 2)
            {
                line = file2.ReadLine();
                while (line != null)
                {
                    lines.Add(line);
                    line = file.ReadLine();
                }
            }
            else
            {
                Console.Write("Invalid choice.");
                return;
            }
            List<int> num = new List<int>();
            foreach (var lin in lines)
            {
                bool setupPT = num.Any(i => i == Int32.Parse(lin.Split('\t')[0]));
                if(!setupPT)
                {
                    num.Add(Int32.Parse(lin.Split('\t')[0]));
                    PageTable t = new PageTable();
                    t.setPageTableNum(Int32.Parse(lin.Split('\t')[0]));
                    tables.Add(t);
                }
            }
            int processNum, vpn, dirty;
            int count = 0;
            int firstmm = 0;
            line = lines[count];
            while (count < lines.Count)//line != null)
            {
                line = lines[count];
                string[] data = line.Split('\t');
               processNum = Convert.ToInt32(data[0]);
               vpn = Convert.ToInt32(data[1]) >> 9;
               char wr = Convert.ToChar(data[2]);
               if (wr == 'W')
                {
                    dirty = 1;
                }
                else
                {
                    dirty = 0;
                }
                PageEntry newp = new PageEntry();
                newp.setDirtybit(dirty);
                newp.setPagetable(processNum);
                newp.setVPN(vpn);
                PageTable t = tables.Find(x => x.getpageTableNum() == newp.getPageTable());
                if (firstmm<32)
                {
                    if (t.getp()[vpn] is null)//mm.getMainMem()[count]==null && count<32)
                    {
                        if (mm.getMainMem()[firstmm] is null)
                        {
                            tables[processNum - 1].getp()[vpn] = newp;
                            mm.EnterPage(firstmm, newp, firstmm);
                            firstmm++;
                            countF++;
                            countR++;
                        }
                    }
                }
                else
                {
                    int check = mm.check(newp.getPageTable(), newp.getVPN());
                    if (check == 0)
                    {
                        countF++;
                        countR++;
                        if (repChoice == 1)//using rand
                        {
                            int rInt = r.Next(0, 32);
                            int md = mm.getMainMem()[rInt].getDirtyBit();
                            if(md == 1)
                            {
                                countD++;
                                countR++;
                            }
                            t.getp()[vpn] = newp;
                            mm.getMainMem()[rInt] = newp;
                        }
                        else if(repChoice == 2)//using fifo
                        {
                            
                        }
                    }
                }
                count++;
            }
            file.Close();
            file2.Close();
            if(repChoice == 1)
            {
                Console.Write("RAND\n");
                Console.Write("Number of Disk References: {0}\n",countR);
                Console.Write("Number of Dirty References: {0}\n", countD);
                Console.Write("Number of Page Faults: {0}\n", countF);
                Console.ReadLine();
            }
        }
    }
}

