public class PageEntry
{
    private int pagetable;
    private int dirty; // 1 for write, 0 for read
    private int VPN;

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
