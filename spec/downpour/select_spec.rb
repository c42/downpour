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

    context "with different data types" do
      before(:each) { @row = @create_results.call("select * from TestFieldTypes").next_row }

      it "should read nil when there is null in the db" do
        @row[0].should be_nil
      end

      it "should read a string" do
        @row[1].should == "foo"
      end

      it "should read an integer" do
        @row[2].should == 42
      end

      it "should read a floating point" do
        @row[3].should == 8.5
      end
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
