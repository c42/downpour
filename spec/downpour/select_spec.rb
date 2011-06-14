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
        @row[1].should be_a(String)
      end

      it "should read an integer" do
        @row[2].should == 42
        @row[2].should be_a(Fixnum)
      end

      it "should read a floating point" do
        @row[3].should == 8.5
        @row[3].should be_a(Float)
      end

      it "should read a long long value" do
        @row[4].should == 2147483648
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
