describe "select queries" do

  before(:each) do
    @status = Downpour.create
    @conn = create_connection(@status)
  end

  shared_examples_for "a select query" do
    it "should read all rows" do
      results = @create_results.call "select * from Test1"
      results.next_row.should == ["foo"]
      results.next_row.should == ["bar"]
      results.next_row.should == ["baz"]
      results.next_row.should be_nil
    end
  end

  context "an buffered select query" do
    before(:each) { @create_results = lambda { |query| @conn.query query }}
    it_should_behave_like "a select query"
  end

  context "an unbuffered select query" do
    before(:each) { @create_results = lambda { |query| @conn.unbuffered_query query }}
    it_should_behave_like "a select query"
  end

  context "a concurrent select query" do
    before(:each) do
      @create_results = lambda do |query|
        @status.add_query @conn, query
        @status.run!.result
      end
    end
    it_should_behave_like "a select query"
  end
end
