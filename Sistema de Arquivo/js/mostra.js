			function criaCabecalho(table)
			{
				if (!table.tHead)
				{
					var header = table.createTHead();
					var Hrow = header.insertRow(-1);

					var Hposicao = Hrow.insertCell(0);
					Hposicao.innerHTML = "<b>#</b>";

					var Hstatus = Hrow.insertCell(1);
					Hstatus.innerHTML = "<b>Status</b>";


					var Hnome = Hrow.insertCell(2);
					Hnome.innerHTML = "<b>Nome</b>";

					var Hdireto = Hrow.insertCell(3);
					Hdireto.innerHTML = "<b>Direto</b>";

					var Hindireto = Hrow.insertCell(4);
					Hindireto.innerHTML = "<b>InDireto</b>";

					var Htamanho = Hrow.insertCell(5);
					Htamanho.innerHTML = "<b>Tamanho</b>";
				}
			}

			function criaLinhas(vetEntradas, table)
			{
				for (var linha = 0; linha < vetEntradas.length; linha++)
				{
					let row = table.insertRow(-1);
					let posicao = row.insertCell(0);
					let status = row.insertCell(1);
					let nome = row.insertCell(2);
					let direto = row.insertCell(3);
					let indireto = row.insertCell(4);
					let tamanho = row.insertCell(5);

					posicao.innerHTML = vetEntradas[linha].posicao;
					status.innerHTML = (vetEntradas[linha].status == 0) ? ('Livre') : ('Ocupado')
					nome.innerHTML = vetEntradas[linha].nome;
					direto.innerHTML   = vetEntradas[linha].direto;
					indireto.innerHTML = (vetEntradas[linha].indireto == 65535) ? ('INVALIDO') : (vetEntradas[linha].indireto);
					tamanho.innerHTML = (vetEntradas[linha].tamanho == 65535) ? ('INVALIDO') : (vetEntradas[linha].tamanho);
				}
			}

			function arrayBufferToString(buffer)
			{
				var byteArray = new Uint8Array(buffer);
				var str = "",
					cc = 0,
					numBytes = 0;

				for (var i = 0, len = byteArray.length; i < len; ++i)
				{
					var v = byteArray[i];
					if (numBytes > 0)
					{
						//2 bit determining that this is a tailing byte + 6 bit of payload
						if ((cc & 192) === 192)
						{
							//processing tailing-bytes
							cc = (cc << 6) | (v & 63);
						}
						else
						{
							throw new Error("this is no tailing-byte");
						}
					}
					else if (v < 128)
					{
						//single-byte
						numBytes = 1;
						cc = v;
					}
					else if (v < 192)
					{
						//these are tailing-bytes
						throw new Error("invalid byte, this is a tailing-byte")
					}
					else if (v < 224)
					{
						//3 bits of header + 5bits of payload
						numBytes = 2;
						cc = v & 31;
					}
					else if (v < 240)
					{
						//4 bits of header + 4bit of payload
						numBytes = 3;
						cc = v & 15;
					}
					else
					{
						//UTF-8 theoretically supports up to 8 bytes containing up to 42bit of payload
						//but JS can only handle 16bit.
						throw new Error("invalid encoding, value out of range")
					}

					if (--numBytes === 0)
					{
						str += String.fromCharCode(cc);
					}
				}
				if (numBytes)
				{
					throw new Error("the bytes don't sum up");
				}
				return str;
			}

			function O(ob)
			{
				return document.getElementById(ob);
			}

			function capturaEntrada(text, posicao, qtd)
			{
				var INICIO = 2 + posicao * 45;
				var obj = {};
				var dados = new Uint8Array(text);

				obj.posicao = posicao;
				obj.status = dados[INICIO];
				obj.nome = arrayBufferToString(dados.slice(INICIO + 1, INICIO + 8));

				 obj.direto = dados.slice(INICIO+9,INICIO+41);

				obj.indireto = ((dados[INICIO + 42] << 8) | dados[INICIO+41]);


				obj.tamanho = ((dados[INICIO+44] << 8) | dados[INICIO+43]);

				return obj;
			}

			function capturaBlocoLivre(text)
			{
				var dados = new Uint8Array(text);
				return ((dados[1] << 8) | dados[0]);
			}

			function criaBlocoPonteiro(ID, valor)
			{
				let iDiv = document.createElement('div');
				iDiv.id = 'blocoPonteiro' + ID;
				iDiv.className = 'blocoPonteiro';

				let divID = document.createElement('div');
				divID.id = 'blocoPonteiroID' + ID;
				divID.className = 'blocoPonteiroID';
				divID.innerHTML = 'bloco:' + ID;

				let divVALUE = document.createElement('div');
				divVALUE.id = 'blocoPonteiroVALUE' + ID;
				divVALUE.className = 'blocoPonteiroVALUE';
                if (valor==65535) valor = "INVALIDO";
				divVALUE.innerHTML = '(' + valor + ')';

				iDiv.appendChild(divID);
				iDiv.appendChild(divVALUE);

				return iDiv;
			}

			function criaBlocoDados(ID, valor)
			{
				let iDiv = document.createElement('div');
				iDiv.id = 'blocoDados' + ID;
				iDiv.className = 'blocoDados';
				iDiv.innerHTML = 'bloco:' + ID;

				iDiv.onclick = function()
				{
                    O('myModalLabel').innerHTML='Conteúdo do bloco de dados:'+ID;
                    O('conteudoBlocoDados').innerHTML=valor;
                    $('#modalDADOS').modal('show');
				};

				return iDiv;
			}

			function leDado(id, text)
			{
				var INICIO = 45*12+2 + 225*2+id*32;
				var dados = new Uint8Array(text);
                var x;
                var texto='';
                for (x=0;x<32;x++) texto = texto + '  ['+dados[INICIO+x]+']';
				return (texto);
			}
      
      function lePonteiro(id, text)
			{
				var INICIO = 45*12+2 + id * 2;
				var dados = new Uint8Array(text);
				return ((dados[INICIO+1] << 8) | dados[INICIO]);
			}

			function abre(event)
			{
				limpaTudo();
				var input = event.target;

				var reader = new FileReader();


				reader.onload = function()
				{

					var text = reader.result;


					// Captura o indicador de primeiro bloco livre
					O('blocoLivre').innerHTML = capturaBlocoLivre(text);


					// Captura as entradas de arquivos
					criaCabecalho(O('tabelaEntradas'));


					var vetorEntradas = Array();
					for (var x = 0; x < 12; x++) vetorEntradas.push(capturaEntrada(text, x, 13));
					criaLinhas(vetorEntradas, O('tabelaEntradas'));


					// Captura blocos ponteiros

					let valor, alvo, iDiv;

					for (var x = 0; x < 225; x++)
					{
						valor = lePonteiro(x, text);
						iDiv = criaBlocoPonteiro(x, valor);

						alvo = O('ponteiros');
						alvo.appendChild(iDiv);
					}



					// Captura Blocos Dados

					for (var x = 0; x < 225; x++)
					{
						//leBlocoDados(x, text);
						let iDiv;
						valor = x + 1;
                        valor = leDado (x,text);
						iDiv = criaBlocoDados(x, valor);

						let alvo = O('dados');
						alvo.appendChild(iDiv);
					}

				};
				reader.readAsArrayBuffer(input.files[0]);
			};

			function limpaTudo()
			{
				O('blocoLivre').innerHTML = '';
				O('tabelaEntradas').innerHTML = '';
				O('dados').innerHTML = '';
			}

