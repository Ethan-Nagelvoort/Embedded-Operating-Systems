using System.Diagnostics;

public class PageEntry
{
    private int pagetable;
    private int dirty; // 1 for write, 0 for read
    private int refr;
    private int used;
    private int VPN;
    private int inMM;
    private Stopwatch timer;
    private int pc;

    public int getpc()
    {
        return this.pc;
    }
    public void setpc(int pp)
    {
        this.pc = pp;
    }
    public void incpc()
    {
        this.pc++;
    }
    public int getU()
    {
        return this.used;
    }
    public void setU(int u)
    {
        this.used = u;
    }
    public int getR()
    {
        return this.refr;
    }
    public void setR(int r)
    {
        this.refr = r;
    }
    public void startT()
    {
        this.timer.Start();
    }

    public Stopwatch getT()
    {
        return this.timer;
    }

    public void setT(Stopwatch t)
    {
        this.timer = t;
    }

    public void setinMM(int MM)
    {
        this.inMM = MM;
    }

    public int getinMM()
    {
        return this.inMM;
    }
    public void setPagetable(int pagetable)
    {
        this.pagetable = pagetable;
    }

    public int getPageTable()
    {
        return this.pagetable;
    }

    public void setDirtybit(int dirty)
    {
        this.dirty = dirty;
    }

    public int getDirtyBit()
    {
        return this.dirty;
    }

    public void setVPN(int VPN)
    {
        this.VPN = VPN;
    }

    public int getVPN()
    {
        return this.VPN;
    }
}
