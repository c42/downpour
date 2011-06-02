describe Downpour do

  before(:each) do
    @status = Downpour.create
    @conn = create_connection(@status)
    @query = @status.add_query(@conn, "select * from Test1 where name like '%oo'");
  end

  it "should not allow query to gc before running" do
    @query = nil
    GC.start
    @status.run!.result.next_row.should include("foo")
  end
end
