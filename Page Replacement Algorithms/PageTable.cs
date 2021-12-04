public class PageTable
{
    //private int index; 
    //private PageEntry newp = new PageEntry();
    private PageEntry[] p = new PageEntry[128];
    /*
    public void setIndex(int index)
    {
        this.index = index;
    }

    public int getIndex()
    {
        return this.index;
    }

    public void setPageEntry(PageEntry newp)
    {
        this.newp = newp;
    }

    public PageEntry getPageEntry()
    {
        return this.newp;
    }
    */
    public void EnterPage(int ind, PageEntry newp)
    {
        p[ind] = newp;
    }

    public int Check(int ind, PageEntry newp)
    {
       if(p[ind] == newp)
        {
            return 1;
        }
       else
        {
            return 2;
        }
    }
}
