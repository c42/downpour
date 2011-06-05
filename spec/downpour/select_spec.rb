describe "a select query" do

  before(:each) do
    @status = Downpour.create
    @conn = create_connection(@status)
    @results = @conn.query "select * from Test1"
  end

  it "should count records" do
    @results.row_count.should == 3
  end

  it "should be able to run a second query" do
    @conn.query "select * from Test2"
  end

  shared_examples_for "a read query" do
    it "should read all rows" do
      @results.next_row.should == ["foo"]
      @results.next_row.should == ["bar"]
      @results.next_row.should == ["baz"]
      @results.next_row.should be_nil
    end
  end

  context "without buffering" do
    # TODO: This API has been removed. See commit 2067c0ef9493f4fe1e877eafea2a0c72e8135391 for more details
    #it_should_behave_like "a read query"

    #after(:each) {@results.should_not be_buffered}
  end

  context "with buffering" do
    it "should be buffered" do
      @results.should be_buffered
    end

    it_should_behave_like "a read query"
  end
end
