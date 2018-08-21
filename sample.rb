
class Rect
  attr_accessor :x,:y,:w,:h
  def include?(rect)
    l = rect.x
    r = l+rect.w
    b = rect.y
    t = b+rect.h
    @x <= r and l <= @x+@w and @y <= t and b <= @y+@w
  end
end

def all_in_mruby(enemies,bullets)
  hitted_enemies = []
  hitted_bullets = []

  start_at = Time.now

  enemies.each{|e|
    i = bullets.find_index{|b| e.include? b }
    if i
      hitted_bullets << bullets.delete_at(i)
      hitted_enemies << e
    end
  }

  end_at = Time.now
  elapse = (end_at - start_at) * 1000
  puts " * start #{start_at} / #{enemies.size}x#{bullets.size} : #{elapse} msec"
  puts " * enemies: #{hitted_enemies.size} / bullets: #{hitted_bullets.size}"
end

def via_instance_value(enemies,bullets)
  hitted_enemies = []
  hitted_bullets = []

  start_at = Time.now

  enemies.each{|e|
    i = e.find_via_iv(bullets)
    if i
      hitted_bullets << bullets.delete_at(i)
      hitted_enemies << e
    end
  }

  end_at = Time.now
  elapse = (end_at - start_at) * 1000
  puts " * start #{start_at} / #{enemies.size}x#{bullets.size} : #{elapse} msec"
  puts " * enemies: #{hitted_enemies.size} / bullets: #{hitted_bullets.size}"
end


def via_struct(enemies,bullets)
  hitted_enemies = []
  hitted_bullets = []

  start_at = Time.now

  enemies.each{|e|
    i = e.find_via_struct(bullets)
    if i
      hitted_bullets << bullets.delete_at(i)
      hitted_enemies << e
    end
  }

  end_at = Time.now
  elapse = (end_at - start_at) * 1000
  puts " * start #{start_at} / #{enemies.size}x#{bullets.size} : #{elapse} msec"
  puts " * enemies: #{hitted_enemies.size} / bullets: #{hitted_bullets.size}"
end

enemies = 100.times.map{ Rect.new( rand(640), rand(480), 32, 32 ) }
bullets = 1000.times.map{ Rect.new( rand(640), rand(480), 2,2 ) }
all_in_mruby enemies.dup, bullets.dup
via_instance_value enemies.dup, bullets.dup
via_struct enemies.dup, bullets.dup

enemies = 1000.times.map{ Rect.new( rand(640), rand(480), 32, 32 ) }
bullets = 100.times.map{ Rect.new( rand(640), rand(480), 2,2 ) }
all_in_mruby enemies.dup, bullets.dup
via_instance_value enemies.dup, bullets.dup
via_struct enemies.dup, bullets.dup


enemies = 100.times.map{ Rect.new( rand(640), rand(480), 32, 32 ) }
bullets = 100.times.map{ Rect.new( rand(640), rand(480), 2,2 ) }
all_in_mruby enemies.dup, bullets.dup
via_instance_value enemies.dup, bullets.dup
via_struct enemies.dup, bullets.dup
