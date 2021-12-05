using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

public class MainMem
{
    private PageEntry[] p = new PageEntry[32];

    public int CheckEm()
    {
        int ch = 0;
        for(int i=0; i<32; i++)
        {
            if(p[i]==null)
            {
                ch= 1;
            }
        }
        return ch;
    }

    public PageEntry[] getMainMem()
    {
        return p;
    }

    public void setMainMem(PageEntry[] pp)
    {
        this.p = pp;
    }
    public void EnterPage(int ind, PageEntry newp, int top)
    {
        int cc = 0;
        for(int i = 0; i<top; i++)
        {
            if((newp.getPageTable()==p[i].getPageTable())&& (newp.getVPN() == p[i].getVPN()))
            {
                cc = 1;
            }
        }
        if (cc == 0)
        {
            p[ind] = newp;
        }
    }
    public int check(int pt, int vpn)
    {
        int cc = 0;
        for (int i = 0; i < 32; i++)
        {
            if ((pt == p[i].getPageTable()) && (vpn == p[i].getVPN()))
            {
                cc = 1;
            }
        }
        return cc;
    }
}
