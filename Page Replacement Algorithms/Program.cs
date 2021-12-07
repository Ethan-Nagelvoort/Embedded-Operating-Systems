  using System;
using System.Collections.Generic;
using System.Collections;
using System.Text;
using System.Linq;
using System.IO;
using System.Diagnostics;
namespace PA4_571
{
    public class Program
    {
        static void Main(string[] args)
        {
            bool g = true;
            while (g)
            {
                System.IO.StreamReader file = new System.IO.StreamReader("data1.txt");
                System.IO.StreamReader file2 = new System.IO.StreamReader("data2.txt");
                MainMem mm = new MainMem();
                mm.setRC(0);
                PageEntry[] mainMem = new PageEntry[32];
                List<PageTable> tables = new List<PageTable>();
                List<string> lines = new List<string>();
                int countF = 0;
                int countR = 0;
                int countD = 0;
                Random r = new Random();
                Console.Write("Enter 1 to read data1. Enter 2 to read data2.\n");
                int choice = Convert.ToInt32(Console.ReadLine());
                Console.Write("Enter 1 to use Rand.\nEnter 2 to use FIFO.\nEnter 3 to use LRU.\nEnter 4 to use PER.\nEnter 5 to use OUR OWN ALGORITHM.\n");
                int repChoice = Convert.ToInt32(Console.ReadLine());
                //parse file into list of strings
                string line = "";
                if (choice == 1)
                {
                    line = file.ReadLine();
                    while (line != null)
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
                        line = file2.ReadLine();
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
                    if (!setupPT)
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
                    newp.setR(0);
                    newp.setU(0);
                    newp.setpc(0);
                    Stopwatch time = new Stopwatch();
                    newp.setT(time);
                    newp.startT();
                    PageTable t = tables.Find(x => x.getpageTableNum() == newp.getPageTable());
                    if (firstmm < 32)
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
                        else
                        {
                            for (int i = 0; i < firstmm; i++)
                            {
                                if ((newp.getPageTable() == mm.getMainMem()[i].getPageTable()) && (vpn == mm.getMainMem()[i].getVPN()))
                                {
                                    mm.getMainMem()[i].getT().Restart();
                                    mm.setRC(mm.getRC() + 1);
                                    if (mm.getRC()>200)
                                    {
                                        for(int f=0; f<tables.Count; f++)
                                        {
                                            for(int ff=0; ff<128; ff++)
                                            {
                                                if (!(tables[f].getp()[ff] is null))
                                                {
                                                    tables[f].getp()[ff].setR(0);
                                                }
                                            }
                                        }
                                    }
                                    mm.resetR();
                                    mm.getMainMem()[i].setR(1);
                                    mm.getMainMem()[i].setU(1);
                                    t.getp()[vpn].setR(1);
                                    t.getp()[vpn].setU(1);
                                    if (newp.getDirtyBit() == 1)
                                    {
                                        t.getp()[vpn].setDirtybit(newp.getDirtyBit());
                                        mm.getMainMem()[i].setDirtybit(newp.getDirtyBit());
                                    }
                                }
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
                                if (md == 1)
                                {
                                    countD++;
                                    countR++;
                                }
                                t.getp()[vpn] = newp;
                                mm.getMainMem()[rInt] = newp;
                            }
                            else if (repChoice == 2)//using fifo
                            {
                                int md = mm.getMainMem()[0].getDirtyBit();
                                if (md == 1)
                                {
                                    countD++;
                                    countR++;
                                }
                                for (int j = 0; j < 31; j++)
                                {
                                    mm.getMainMem()[j] = mm.getMainMem()[j + 1];
                                }
                                t.getp()[vpn] = newp;
                                mm.getMainMem()[31] = newp;
                            }
                            else if (repChoice == 3)//LRU
                            {
                                int ind = 0;
                                for(int i=0; i<32; i++)
                                {

                                    if(mm.getMainMem()[ind].getT().Elapsed.TotalMilliseconds < mm.getMainMem()[i].getT().Elapsed.TotalMilliseconds)
                                    {
                                        ind = i;
                                    }
                                }
                                for (int i = 0; i < 32; i++)
                                {

                                    if ((mm.getMainMem()[ind].getT().Elapsed.TotalMilliseconds == mm.getMainMem()[i].getT().Elapsed.TotalMilliseconds) && (ind != i))
                                    {
                                        if(mm.getMainMem()[ind].getDirtyBit()==1 && mm.getMainMem()[i].getDirtyBit() == 0)
                                        {
                                            ind = i;
                                        }
                                        else if(mm.getMainMem()[ind].getDirtyBit() == 0 && mm.getMainMem()[i].getDirtyBit() == 1)
                                        {
                                            ind = ind;
                                        }
                                        else if(mm.getMainMem()[i].getVPN() < mm.getMainMem()[ind].getVPN())
                                        {
                                            ind = i;
                                        }
                                        else
                                        {
                                            ind = ind;
                                        }
                                    }
                                }
                                int md = mm.getMainMem()[ind].getDirtyBit();
                                if (md == 1)
                                {
                                    countD++;
                                    countR++;
                                }
                                t.getp()[vpn] = newp;
                                mm.getMainMem()[ind] = newp;
                            }
                            else if(repChoice==4) //PER
                            {
                                int ind = 55;
                                for (int i = 0; i < 32; i++)
                                {
                                    if(mm.getMainMem()[i].getU()==0)
                                    {
                                        if(ind<32)
                                        {
                                            if(mm.getMainMem()[i].getVPN()< mm.getMainMem()[ind].getVPN())
                                            {
                                                ind = i;
                                            }
                                        }
                                        else
                                        {
                                            ind = i;
                                        }
                                    }
                                }
                                if (ind > 31)
                                {
                                    for (int i = 0; i < 32; i++)
                                    {
                                        if (mm.getMainMem()[i].getR() == 0 && mm.getMainMem()[i].getDirtyBit() == 0)
                                        {
                                            if (ind < 32)
                                            {
                                                if (mm.getMainMem()[i].getVPN() < mm.getMainMem()[ind].getVPN())
                                                {
                                                    ind = i;
                                                }
                                            }
                                            else
                                            {
                                                ind = i;
                                            }
                                        }
                                    }
                                }
                                if (ind > 31)
                                {
                                    for (int i = 0; i < 32; i++)
                                    {
                                        if (mm.getMainMem()[i].getR() == 0 && mm.getMainMem()[i].getDirtyBit() == 1)
                                        {
                                            if (ind < 32)
                                            {
                                                if (mm.getMainMem()[i].getVPN() < mm.getMainMem()[ind].getVPN())
                                                {
                                                    ind = i;
                                                }
                                            }
                                            else
                                            {
                                                ind = i;
                                            }
                                        }
                                    }
                                }
                                if (ind > 31)
                                {
                                    for (int i = 0; i < 32; i++)
                                    {
                                        if (mm.getMainMem()[i].getR() == 1 && mm.getMainMem()[i].getDirtyBit() == 0)
                                        {
                                            if (ind < 32)
                                            {
                                                if (mm.getMainMem()[i].getVPN() < mm.getMainMem()[ind].getVPN())
                                                {
                                                    ind = i;
                                                }
                                            }
                                            else
                                            {
                                                ind = i;
                                            }
                                        }
                                    }
                                }
                                if (ind > 31)
                                {
                                    for (int i = 0; i < 32; i++)
                                    {
                                        if (mm.getMainMem()[i].getR() == 1 && mm.getMainMem()[i].getDirtyBit() == 1)
                                        {
                                            if (ind < 32)
                                            {
                                                if (mm.getMainMem()[i].getVPN() < mm.getMainMem()[ind].getVPN())
                                                {
                                                    ind = i;
                                                }
                                            }
                                            else
                                            {
                                                ind = i;
                                            }
                                        }
                                    }
                                }
                                int md = mm.getMainMem()[ind].getDirtyBit();
                                if (md == 1)
                                {
                                    countD++;
                                    countR++;
                                }
                                t.getp()[vpn] = newp;
                                mm.getMainMem()[ind] = newp;
                            }
                            else//our own algorithm
                            {
                                int pcount = count+1;
                                int flag = 0;
                                while (flag == 0 &&  pcount<lines.Count) //flag == 0 &&
                                {
                                    string pline = lines[pcount];
                                    string[] pdata = pline.Split('\t');
                                    int ptable = Convert.ToInt32(pdata[0]);
                                    int pvpn = Convert.ToInt32(pdata[1]) >> 9;
                                    for(int i=0; i<32; i++)
                                    {
                                        if(mm.getMainMem()[i].getPageTable()==ptable && mm.getMainMem()[i].getVPN() == pvpn)
                                        {
                                            mm.getMainMem()[i].incpc();
                                        }
                                    }
                                    pcount++;
                                    if(pcount == count+500)
                                    {
                                        flag = 1;
                                    }
                                }
                                int ind = 0;
                                for(int i=0; i<32; i++)
                                {
                                    if(mm.getMainMem()[i].getpc() < mm.getMainMem()[ind].getpc())
                                    {
                                        ind = i;
                                    }
                                }
                                int md = mm.getMainMem()[ind].getDirtyBit();
                                if (md == 1)
                                {
                                    countD++;
                                    countR++;
                                }
                                t.getp()[vpn] = newp;
                                mm.getMainMem()[ind] = newp;
                                for(int i=0; i<32; i++)
                                {
                                    mm.getMainMem()[i].setpc(0);
                                }
                            }
                        }

                        else
                        {
                            for (int i = 0; i < 32; i++)
                            {
                                if ((newp.getPageTable() == mm.getMainMem()[i].getPageTable()) && (vpn == mm.getMainMem()[i].getVPN()))
                                {
                                    mm.getMainMem()[i].getT().Restart();
                                    mm.setRC(mm.getRC() + 1);
                                    if (mm.getRC() > 200)
                                    {
                                        for (int f = 0; f < tables.Count; f++)
                                        {
                                            for (int ff = 0; ff < 128; ff++)
                                            {
                                                if (!(tables[f].getp()[ff] is null))
                                                {
                                                    tables[f].getp()[ff].setR(0);
                                                }
                                            }
                                        }
                                    }
                                    mm.resetR();
                                    mm.getMainMem()[i].setR(1);
                                    mm.getMainMem()[i].setU(1);
                                    t.getp()[vpn].setR(1);
                                    t.getp()[vpn].setU(1);
                                    if (newp.getDirtyBit() == 1)
                                    {
                                        t.getp()[vpn].setDirtybit(newp.getDirtyBit());
                                        mm.getMainMem()[i].setDirtybit(newp.getDirtyBit());
                                    }
                                }
                            }
                        }
                    }
                    count++;
                }
                file.Close();
                file2.Close();
                if (repChoice == 1)
                {
                    Console.Write("RAND\n");
                    Console.Write("Number of Page Faults: {0}\n", countF);
                    Console.Write("Number of Dirty Page Writes: {0}\n", countD);
                    Console.Write("Number of Disk Accesses: {0}\n", countR);
                    //Console.ReadLine();
                }
                if (repChoice == 2)
                {
                    Console.Write("FIFO\n");
                    Console.Write("Number of Page Faults: {0}\n", countF);
                    Console.Write("Number of Dirty Page Writes: {0}\n", countD);
                    Console.Write("Number of Disk Accesses: {0}\n", countR);
                    //Console.ReadLine();
                }
                if (repChoice == 3)
                {
                    Console.Write("LRU\n");
                    Console.Write("Number of Page Faults: {0}\n", countF);
                    Console.Write("Number of Dirty Page Writes: {0}\n", countD);
                    Console.Write("Number of Disk Accesses: {0}\n", countR);
                    Console.ReadLine();
                }
                if (repChoice == 4)
                {
                    Console.Write("PER\n");
                    Console.Write("Number of Page Faults: {0}\n", countF);
                    Console.Write("Number of Dirty Page Writes: {0}\n", countD);
                    Console.Write("Number of Disk Accesses: {0}\n", countR);
                    Console.ReadLine();
                }
                if (repChoice == 5)
                {
                    Console.Write("OUR OWN ALGORITHM\n");
                    Console.Write("Number of Page Faults: {0}\n", countF);
                    Console.Write("Number of Dirty Page Writes: {0}\n", countD);
                    Console.Write("Number of Disk Accesses: {0}\n", countR);
                    Console.ReadLine();
                }
            }
        }
    }
}  

