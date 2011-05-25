require 'spec_helper'

describe Drizzle do
  it "should have the version number set" do
    Drizzle.version.should == "7"
  end
end
