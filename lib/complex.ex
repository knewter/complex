defmodule Complex do
  ## Public API
  def start(external_program) do
    spawn(__MODULE__, :init, [external_program])
  end

  def stop do
    send(:complex, :stop)
  end

  def foo(x) do
    call_port({:foo, x})
  end

  def bar(y) do
    call_port({:bar, y})
  end

  def call_port(message) do
    send(:complex, {:call, self, message})
    receive do
      {:complex, result} -> result
    end
  end

  ## Server API

  def init(external_program) do
    Process.register(self, :complex)
    Process.flag(:trap_exit, true)
    port = Port.open({:spawn, external_program}, [packet: 2])
    loop(port)
  end

  ## Private bits
  def loop(port) do
    receive do
      {:call, caller, message} ->
        send(port, {self, {:command, encode(message)}})
        receive do
          {^port, {:data, data}} ->
            send(caller, {:complex, decode(data)})
        end
        loop(port)
      :stop ->
        send(port, {self, :close})
        receive do
          {^port, :closed} ->
            exit(:normal)
        end
      {'EXIT', ^port, _reason} ->
        exit(:port_terminated)
    end
  end

  def encode({:foo, x}), do: [1, x]
  def encode({:bar, y}), do: [2, y]

  def decode([int]), do: int
end
