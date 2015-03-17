#!/usr/bin/env ruby

module Enumerable

  def sum
    self.inject(0) {|accum, i|
      accum + i
    }
  end

  def mean
    self.sum / self.length.to_f
  end

  def sample_variance
    m = self.mean
    sum = self.inject(0) { |accum, i| accum + (i - m) ** 2 }
    sum / (self.length - 1).to_f
  end

  def standard_deviation
    return Math.sqrt(self.sample_variance)
  end

  def ci
    return self.standard_deviation * 2.776 / Math.sqrt(5)
  end
end

ARGV.each do |filename|
  times = []
  File.foreach(filename) {|line|
    if not /Elapsed/.match(line)
      next
    end

    /(\d+):(.*)$/.match(line)
    m = $1
    m = m.to_f * 60
    s = $2
    s = s.to_f

    times.push(m + s)
  }

  puts times.ci
  puts times.mean
end

