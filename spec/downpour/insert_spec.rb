describe "an insert query" do

  def count_of_test2
    @conn.query("select * from Test2").row_count
  end

  before(:each) do
    @status = Downpour.create
    @conn = create_connection(@status)
  end

  it "should increment the row count by one" do
    lambda { @conn.query "insert into Test2 (name) values ('foo')" }.should change(self, :count_of_test2).by(1)
  end

  it "should get id after inserting" do
    @conn.query("insert into Test2 (name) values ('foo')").insert_id.should == count_of_test2
  end
end
