public class PageTable
{
    //private int index; 
    //private PageEntry newp = new PageEntry();
    private PageEntry[] p = new PageEntry[128];
    private int pageTableNum;

    public void setPageTableNum(int pn)
    {
        this.pageTableNum = pn;
    }
    public int getpageTableNum()
    {
        return this.pageTableNum;
    }
    public PageEntry[] getp()
    {
        return this.p;
    }

    public void setp(PageEntry[] pp)
    {
        this.p = pp;
    }
    public int Check(int ind, PageEntry newp)
    {
        if (p[ind].getVPN() == newp.getVPN())
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }
}
